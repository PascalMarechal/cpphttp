#include "body.h"

using namespace cpphttp::request;

body::body(const std::string &data) : m_data(data)
{
}

const std::string &body::getData() const noexcept
{
    return m_data;
}

std::size_t body::getSize() const noexcept
{
    return m_data.size();
}

namespace cpphttp
{
    namespace request
    {
        bool operator==(const body &lhs, const body &rhs) noexcept
        {
            return lhs.getSize() == rhs.getSize() &&
                   lhs.m_data == rhs.m_data;
        }

        bool operator!=(const body &lhs, const body &rhs) noexcept
        {
            return !(lhs == rhs);
        }
    } // namespace request
} // namespace cpphttp
