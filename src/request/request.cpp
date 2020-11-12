#include "request.h"

using namespace cpphttp::request;

bool request::isReady() const noexcept
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

const header *request::getHeader() const noexcept
{
    return m_header.get();
}

void request::setHeader(const std::string &data) noexcept
{
    m_header = std::make_unique<header>(data);
}
void request::setBody(const std::string &data) noexcept
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
