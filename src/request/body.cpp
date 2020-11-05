#include "body.h"

using namespace cpphttp::request;

void body::appendData(const std::string &data)
{
    m_data += data;
}

std::string &body::getData() noexcept
{
    return m_data;
}

std::size_t body::getSize() noexcept
{
    return m_data.size();
}
