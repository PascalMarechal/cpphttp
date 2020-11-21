#pragma once

#include <memory>
#include "router.h"

namespace cpphttp
{
    namespace server
    {
        class server
        {
        public:
            server(uint32_t port);
            ~server();

            void start();
            void stop();
        private:
            class impl;
            std::unique_ptr<impl> m_server_impl;
        };
    } // namespace server
} // namespace cpphttp
