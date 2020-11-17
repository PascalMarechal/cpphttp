#include "body.h"

using namespace cpphttp::request;

class body::impl
{
public:
    impl(const std::string &data) : m_data(data)
    {
    }

    const std::string &getData() const noexcept
    {
        return m_data;
    }

    std::size_t getSize() const noexcept
    {
        return m_data.size();
    }

    friend bool operator==(const impl &lhs, const impl &rhs) noexcept
    {
        return lhs.getSize() == rhs.getSize() &&
               lhs.m_data == rhs.m_data;
    }

private:
    std::string m_data;
};

body::body(const std::string &data) : m_impl(std::make_unique<impl>(data))
{
}

body::~body(){}

const std::string &body::getData() const noexcept
{
    return m_impl->getData();
}

std::size_t body::getSize() const noexcept
{
    return m_impl->getSize();
}

namespace cpphttp
{
    namespace request
    {
        bool operator==(const body &lhs, const body &rhs) noexcept
        {
            return *lhs.m_impl == *rhs.m_impl;
        }

        bool operator!=(const body &lhs, const body &rhs) noexcept
        {
            return !(lhs == rhs);
        }
    } // namespace request
} // namespace cpphttp
