#include "response.h"

using namespace cpphttp::response;

void response::status(cpphttp::response::status s) noexcept
{
    m_header.setStatus(s);
}

void response::setHeaderLength() noexcept
{
    m_header.setContentLength(m_body.size());
}

std::string response::toString() noexcept
{
    setHeaderLength();
    return m_header.toString() + m_body;
}

void response::write(const std::string &data) noexcept
{
    m_body += data;
}
