#include "response.h"
#include <sstream>

using namespace cpphttp::response;

class response::impl
{
public:
    void status(cpphttp::response::status s) noexcept
    {
        m_header.status(s);
    }
    std::string toString() const noexcept
    {
        return m_header.toString() + m_body.str();
    }
    void write(const std::string &data) noexcept
    {
        m_bodyLength += data.size();
        m_header.setContentLength(m_bodyLength);
        m_body << data;
    }

private:
    header m_header;
    std::ostringstream m_body;
    uint32_t m_bodyLength;
};

response::response() : m_impl(std::make_unique<impl>())
{
}

response::~response() {}

void response::status(cpphttp::response::status s) noexcept
{
    m_impl->status(s);
}

std::string response::toString() const noexcept
{
    return m_impl->toString();
}

void response::write(const std::string &data) noexcept
{
    m_impl->write(data);
}
