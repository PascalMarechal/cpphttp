/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include <memory>
#include "method.h"
#include "version.h"

#define MAX_HEADER_SIZE 8 * 1024

namespace cpphttp
{
    namespace request
    {
        class header
        {
        public:
            header(const std::string_view &data);
            ~header();

            bool isReady() const noexcept;
            cpphttp::request::method method() const noexcept;
            cpphttp::request::version version() const noexcept;
            const std::string &path() const noexcept;
            const std::string &getParams() const noexcept;
            uint32_t expectedBodySize() const noexcept;

            friend bool operator==(const header &lhs, const header &rhs) noexcept;
            friend bool operator!=(const header &lhs, const header &rhs) noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace request
} // namespace cpphttp
