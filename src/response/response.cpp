/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "response.h"
#include <sstream>

using namespace cpphttp::response;

class response::impl
{
public:
    impl() : m_bodyLength(0), m_end(false) {}

    inline void status(cpphttp::response::status s) noexcept
    {
        m_header.status(s);
    }

    inline std::string toString() const noexcept
    {
        return m_header.toString() + m_body.str();
    }

    inline void write(const std::string &data) noexcept
    {
        if (hasEnded())
            return;
        m_bodyLength += data.size();
        m_header.setContentLength(m_bodyLength);
        m_body << data;
    }

    inline void send(const std::string &data) noexcept
    {
        write(data);
        end();
    }

    inline void end() noexcept
    {
        m_end = true;
    }

    inline bool hasEnded() const noexcept
    {
        return m_end;
    }

    inline cpphttp::response::header &header() noexcept
    {
        return m_header;
    }

private:
    cpphttp::response::header m_header;
    std::ostringstream m_body;
    uint32_t m_bodyLength;
    bool m_end;
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

void response::end() noexcept
{
    m_impl->end();
}

bool response::hasEnded() const noexcept
{
    return m_impl->hasEnded();
}

void response::send(const std::string &data) noexcept
{
    m_impl->send(data);
}

header &response::header() noexcept
{
    return m_impl->header();
}
