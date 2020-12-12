/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "body.h"

using namespace cpphttp::request;

class body::impl
{
public:
    impl(const std::string_view &data) : m_data(data)
    {
    }

    const std::string &data() const noexcept
    {
        return m_data;
    }

    std::size_t size() const noexcept
    {
        return m_data.size();
    }

    friend bool operator==(const impl &lhs, const impl &rhs) noexcept
    {
        return lhs.size() == rhs.size() &&
               lhs.m_data == rhs.m_data;
    }

private:
    std::string m_data;
};

body::body(const std::string_view &data) : m_impl(std::make_unique<impl>(data))
{
}

body::~body() {}

const std::string &body::data() const noexcept
{
    return m_impl->data();
}

std::size_t body::size() const noexcept
{
    return m_impl->size();
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
