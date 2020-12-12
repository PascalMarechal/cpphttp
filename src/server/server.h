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
            void setMaxIncomingHeaderSize(u_int64_t size) noexcept;
            void setMaxIncomingBodySize(u_int64_t size) noexcept;

            void publicFolder(const std::string &path, const std::string &folderPath);
            const std::string &publicFolderPath() const noexcept;
            const std::string &publicFolderURL() const noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_server_impl;
        };
    } // namespace server
} // namespace cpphttp
