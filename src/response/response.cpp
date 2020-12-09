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
    impl() : m_end(false) {}

    inline void status(cpphttp::response::status s) noexcept
    {
        m_header.status(s);
    }

    inline std::string toString() const noexcept
    {
        return m_header.toString() + std::string(m_body.begin(), m_body.end());
    }

    inline void write(const std::string &data) noexcept
    {
        if (hasEnded())
            return;
        m_body.insert(m_body.end(), data.cbegin(), data.cend());
        m_header.setContentLength(m_body.size());
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

    inline std::vector<uint8_t> toVector() const noexcept
    {
        auto result = m_header.toVector();
        result.insert(result.end(), m_body.cbegin(), m_body.cend());
        return result;
    }

private:
    cpphttp::response::header m_header;
    std::vector<uint8_t> m_body;
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

std::vector<uint8_t> response::toVector() const noexcept
{
    return m_impl->toVector();
}
