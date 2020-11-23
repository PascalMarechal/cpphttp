#pragma once
#include <functional>
#include "request/request.h"

namespace cpphttp
{
    namespace internal
    {
        template <typename Socket, typename ConnectionFunctions, typename Router>
        class connection : public std::enable_shared_from_this<connection<Socket, ConnectionFunctions, Router>>
        {
        public:
            connection(Socket &&sock, const ConnectionFunctions &functions, const Router &router)
                : m_socket(std::move(sock)), m_functions(functions), m_router(router){};

            void start()
            {
                readHeader();
            }

        private:
            inline void readHeader()
            {
                m_functions.async_read_header(m_socket, m_functions.createBuffer(m_buffer), m_functions.headerEndMatcher(), std::bind(&connection::onReadHeader, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            inline void readBody()
            {
                m_functions.async_read_body(m_socket, m_functions.createBuffer(m_buffer), m_functions.bodyEndMatcher(m_toRead), std::bind(&connection::onReadBody, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            void processAndReadNextRequest()
            {
                m_buffer.clear();
                auto response = m_router.process(m_currentRequest);
                m_functions.write(m_socket, response);
                readHeader();
            }

            void exit(std::error_code &error)
            {
                m_socket.close(error);
            }

            void onReadHeader(std::error_code error, std::size_t bytes_transferred)
            {
                if (error)
                    return exit(error);

                std::string_view headerView = m_buffer;
                m_headerSize = bytes_transferred;
                m_currentRequest.setHeader(headerView.substr(0, bytes_transferred));

                if (!m_currentRequest.header().isReady())
                    return exit(error);

                auto expectedBodySize = m_currentRequest.header().getExpectedBodySize();
                if (expectedBodySize > m_functions.maxBodySize())
                    return exit(error);

                if (expectedBodySize > 0)
                {
                    auto extra = m_buffer.size() - bytes_transferred;
                    if (extra < expectedBodySize)
                    {
                        m_toRead = expectedBodySize - extra;
                        readBody();
                    }
                    else
                    {
                        m_currentRequest.setBody(headerView.substr(bytes_transferred, expectedBodySize));
                        processAndReadNextRequest();
                    }
                }
                else
                    processAndReadNextRequest();
            }

            void onReadBody(std::error_code error, std::size_t bytes_transferred)
            {
                if (error)
                    return exit(error);
                m_currentRequest.setBody(std::string_view(m_buffer).substr(m_headerSize, m_currentRequest.header().getExpectedBodySize()));
                processAndReadNextRequest();
            }

            Socket m_socket;
            const ConnectionFunctions &m_functions;
            const Router &m_router;

            std::string m_buffer;
            std::size_t m_toRead;
            std::size_t m_headerSize;
            cpphttp::request::request m_currentRequest;
        };
    } // namespace internal
} // namespace cpphttp
