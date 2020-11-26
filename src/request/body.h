/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */

#pragma once

#include <memory>

namespace cpphttp
{
    namespace request
    {
        class body
        {
        public:
            body(const std::string_view &data);
            ~body();
            const std::string &getData() const noexcept;
            std::size_t getSize() const noexcept;

            friend bool operator==(const body &lhs, const body &rhs) noexcept;
            friend bool operator!=(const body &lhs, const body &rhs) noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace request
} // namespace cpphttp
