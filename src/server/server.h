/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
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
            ~server() noexcept;

            void start() noexcept;
            void stop() noexcept;
            void setRouter(router &&router) noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_server_impl;
        };
    } // namespace server
} // namespace cpphttp
