#pragma once

#include <string>

namespace cpphttp
{
    namespace request
    {
        class body
        {
        public:
            body(const std::string &data);
            const std::string &getData() const noexcept;
            std::size_t getSize() const noexcept;

            friend bool operator==(const body &lhs, const body &rhs) noexcept;
            friend bool operator!=(const body &lhs, const body &rhs) noexcept;

        private:
            std::string m_data;
        };
    } // namespace request
} // namespace cpphttp
