#pragma once
#include <functional>
#include "internal/asio.h"
#include "request/request.h"
#include "match_end_of_header.h"

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
                m_functions.async_read_until(m_socket, m_functions.createBuffer(m_headerBuffer), m_matcher, std::bind(&connection::onReadHeader, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            inline void readBody()
            {
                m_functions.async_read_exactly(m_socket, m_functions.createBuffer(m_bodyBuffer), asio::transfer_exactly(m_currentRequest.header().getExpectedBodySize()), std::bind(&connection::onReadBody, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            void processAndReadNextRequest()
            {
                auto response = m_router.process(m_currentRequest);
                m_functions.write(m_socket, response);
                readHeader();
            }

            void onReadHeader(std::error_code error, std::size_t bytes_transferred)
            {
                if (error)
                    return m_socket.close(error);

                m_currentRequest.setHeader(m_headerBuffer);
                m_headerBuffer.clear();

                if (!m_currentRequest.header().isReady())
                    return m_socket.close(error);

                if (m_currentRequest.header().getExpectedBodySize() > 0)
                    readBody();
                else
                    processAndReadNextRequest();
            }

            void onReadBody(std::error_code error, std::size_t bytes_transferred)
            {
                if (error)
                    return m_socket.close(error);

                m_currentRequest.setBody(m_bodyBuffer);
                m_bodyBuffer.clear();
                processAndReadNextRequest();
            }

            Socket m_socket;
            std::string m_headerBuffer;
            std::string m_bodyBuffer;
            const ConnectionFunctions &m_functions;
            const Router &m_router;

            static inline match_end_of_header m_matcher;
            cpphttp::request::request m_currentRequest;
        };
    } // namespace internal
} // namespace cpphttp
