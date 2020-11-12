#include "request.h"

using namespace cpphttp::request;

bool request::isReady()
{
    if (m_header && m_header->isReady())
    {
        if (!m_body)
            return m_header->getExpectedBodySize() == 0;
        else
            return m_header->getExpectedBodySize() == m_body->getSize();
    }
    return false;
}

void request::setHeader(const std::string &data)
{
    m_header = std::make_unique<header>(data);
}
void request::setBody(const std::string &data)
{
    m_body = std::make_unique<body>(data);
}

request::request(request &&toCopy) noexcept
{
    m_header = std::move(toCopy.m_header);
    m_body = std::move(toCopy.m_body);
}

request &request::operator=(request &&toCopy) noexcept
{
    if (this == &toCopy)
        return *this;
    m_header = std::move(toCopy.m_header);
    m_body = std::move(toCopy.m_body);
    return *this;
}
