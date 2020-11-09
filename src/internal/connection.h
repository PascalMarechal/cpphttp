#pragma once
#include <memory>
#include "asio.h"

namespace cpphttp
{
    namespace internal
    {
        class connection
        {
        public:
            connection(asio::ip::tcp::socket &&socket);

        private:
            asio::ip::tcp::socket m_socket;
        };
    } // namespace server
} // namespace cpphttp
