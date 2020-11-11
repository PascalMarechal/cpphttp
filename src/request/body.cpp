#include "body.h"

using namespace cpphttp::request;

body::body(const std::string& data):m_data(data)
{
}

const std::string &body::getData() noexcept
{
    return m_data;
}

std::size_t body::getSize() noexcept
{
    return m_data.size();
}
