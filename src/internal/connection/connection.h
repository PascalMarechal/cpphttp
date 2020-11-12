#pragma once
#include <functional>
#include "internal/asio.h"
#include "request/request.h"
#include "match_end_of_header.h"

namespace cpphttp
{
    namespace internal
    {
        template <typename ConnectionFunctions, typename Router>
        class connection : public std::enable_shared_from_this<connection<ConnectionFunctions, Router>>
        {
        public:
            connection(asio::ip::tcp::socket &&sock, const ConnectionFunctions &functions, const Router &router)
                : m_socket(std::move(sock)), m_functions(functions), m_router(router){};

            void start()
            {
                readHeader();
            }

        private:
            inline void readHeader()
            {
                m_functions.async_read_until(m_socket, asio::dynamic_buffer(m_headerBuffer), m_matcher, std::bind(&connection::onReadHeader, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            inline void readBody()
            {
                m_currentRequest.setHeader(m_headerBuffer);
                m_headerBuffer.clear();
                m_functions.async_read_exactly(m_socket, asio::dynamic_buffer(m_bodyBuffer), asio::transfer_exactly(m_currentRequest.getHeader()->getExpectedBodySize()), std::bind(&connection::onReadBody, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            void onReadHeader(asio::error_code error, std::size_t bytes_transferred)
            {
                readBody();
            }

            void onReadBody(asio::error_code error, std::size_t bytes_transferred)
            {
                m_functions.async_work(m_socket.get_executor(), [from = this->shared_from_this()]() mutable -> void {
                    from->processRequests();
                });
            }

            void processRequests()
            {
                m_router.process(m_currentRequest);
            }

            asio::ip::tcp::socket m_socket;
            std::string m_headerBuffer;
            std::string m_bodyBuffer;
            const ConnectionFunctions &m_functions;
            const Router &m_router;

            static inline match_end_of_header m_matcher;
            cpphttp::request::request m_currentRequest;
        };
    } // namespace internal
} // namespace cpphttp
