#pragma once
#include <memory>
#include "asio.h"
#include "request/request.h"

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
                m_async_functions.async_read_until(
                    m_socket, asio::buffer(m_headerBuffer, MAX_HEADER_SIZE), "\n", [this](std::error_code error, std::size_t bytes_transferred)
                    {
                        on_read(error, bytes_transferred);
                    });
            }

        private:
            inline void on_read(std::error_code error, std::size_t bytes_transferred)
            {
                std::cout << "READ\n";
            }

            asio::ip::tcp::socket m_socket;
            AsyncFunctions m_async_functions;
            char m_headerBuffer[MAX_HEADER_SIZE];
        };
    } // namespace internal
} // namespace cpphttp
