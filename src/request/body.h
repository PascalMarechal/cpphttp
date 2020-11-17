#pragma once

#include <memory>

namespace cpphttp
{
    namespace request
    {
        class body
        {
        public:
            body(const std::string &data);
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
