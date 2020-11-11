#pragma once
#include <functional>
#include "internal/asio.h"
//#include "request/request.h"
#include "match_end_of_header.h"

namespace cpphttp
{
    namespace internal
    {
        template <typename AsyncFunctions>
        class connection : public std::enable_shared_from_this<connection<AsyncFunctions>>
        {
        public:
            using buffer_iterator = asio::buffers_iterator<asio::streambuf::const_buffers_type>;

            connection(asio::ip::tcp::socket &&sock) : m_socket(std::move(sock)){};

            void start()
            {
                m_async_functions.async_read_until(m_socket, asio::dynamic_buffer(m_headerBuffer), m_matcher, std::bind(&connection::onRead, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
                //asio::async_read_until(m_socket, asio::dynamic_buffer(m_headerBuffer), matcher, std::bind(&connection::onRead, this, std::placeholders::_1, std::placeholders::_2));
            }

        private:
            void onRead(asio::error_code error, std::size_t bytes_transferred)
            {
                std::cout << "READ\n";
            }

            asio::ip::tcp::socket m_socket;
            std::string m_headerBuffer;

            AsyncFunctions m_async_functions;
            static inline match_end_of_header m_matcher;
        };
    } // namespace internal
} // namespace cpphttp
