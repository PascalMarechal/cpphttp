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
                //asio::async_read_until(m_socket, asio::dynamic_buffer(m_headerBuffer), matcher, std::bind(&connection::onRead, this, std::placeholders::_1, std::placeholders::_2));
            }

        private:
            inline void readHeader()
            {
                m_functions.async_read_until(m_socket, asio::dynamic_buffer(m_headerBuffer), m_matcher, std::bind(&connection::onReadHeader, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            inline void readBody()
            {
                m_currentReq.setHeader(m_headerBuffer);
                m_headerBuffer.clear();
                m_functions.async_read_exactly(m_socket, asio::dynamic_buffer(m_bodyBuffer), asio::transfer_exactly(m_currentReq.getHeader()->getExpectedBodySize()), std::bind(&connection::onReadBody, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            void onReadHeader(asio::error_code error, std::size_t bytes_transferred)
            {
                readBody();
            }

            void onReadBody(asio::error_code error, std::size_t bytes_transferred)
            {
                // asio::post(m_socket.get_executor(), [reqCapute = std::move(this->m_currentReq), this]() mutable -> void {
                //     std::cout << "LOL";
                //     m_router.process(std::move(reqCapute));
                // });
                m_functions.async_work(m_socket.get_executor(), [this]() mutable -> void {
                    this->processRequests();
                });
            }

            void processRequests()
            {
                m_router.process(std::move(m_currentReq));
            }

            asio::ip::tcp::socket m_socket;
            std::string m_headerBuffer;
            std::string m_bodyBuffer;
            const ConnectionFunctions &m_functions;
            const Router &m_router;

            static inline match_end_of_header m_matcher;
            cpphttp::request::request m_currentReq;
        };
    } // namespace internal
} // namespace cpphttp
