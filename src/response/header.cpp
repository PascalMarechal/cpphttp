/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "header.h"
#include <unordered_map>
#include <sstream>

using namespace cpphttp;
using namespace response;

class header::impl
{
public:
    impl() : m_status(status::_200), m_contentLength(0), m_contentType(DEFAULT_CONTENT_TYPE)
    {
    }
    inline request::version version() const noexcept
    {
        return request::version::_1_1;
    }
    inline response::status status() const noexcept
    {
        return m_status;
    }
    inline uint32_t contentLength() const noexcept
    {
        return m_contentLength;
    }
    inline void status(response::status status) noexcept
    {
        m_status = status;
    }
    inline void setContentLength(uint32_t length) noexcept
    {
        m_contentLength = length;
    }
    inline std::string toString() const noexcept
    {
        std::ostringstream result;

        auto statusText = STATUS_MAPPING.find(m_status);
        result << "HTTP/1.1 " << statusText->second
               << "\r\nContent-Type:" << m_contentType
               << "\r\nContent-Length: " << m_contentLength
               << "\r\nConnection: Keep-Alive\r\n\r\n";
        return result.str();
    }

    inline const std::string &getContentType() const noexcept
    {
        return m_contentType;
    }

    inline void setContentType(const std::string &type) noexcept
    {
        m_contentType = type;
    }

    inline std::vector<uint8_t> toVector() const noexcept
    {
        auto stringValue = toString();
        return std::vector<uint8_t>(stringValue.begin(), stringValue.end());
    }

private:
    response::status m_status;
    uint32_t m_contentLength;
    std::string m_contentType;

    const inline static std::unordered_map<response::status, std::string> STATUS_MAPPING =
        {{status::_200, "200 OK"},
         {status::_201, "201 Created"},
         {status::_202, "202 Accepted"},
         {status::_203, "203 Non-Authoritative Information"},
         {status::_204, "204 No Content"},
         {status::_205, "205 Reset Content"},
         {status::_206, "206 Partial Content"},
         {status::_207, "207 Multi-Status"},
         {status::_208, "208 Already Reported"},
         {status::_210, "210 Content Different"},
         {status::_226, "226 IM Used"},
         {status::_301, "301 Moved Permanently"},
         {status::_400, "400 Bad Request"},
         {status::_401, "401 Unauthorized"},
         {status::_403, "403 Forbidden"},
         {status::_404, "404 Not Found"},
         {status::_405, "405 Method Not Allowed"},
         {status::_500, "500 Internal Server Error"},
         {status::_501, "501 Not Implemented"}};
};

header::header() : m_impl(std::make_unique<impl>())
{
}

header::~header() {}

request::version header::version() const noexcept
{
    return m_impl->version();
}

status header::status() const noexcept
{
    return m_impl->status();
}

void header::status(response::status s) noexcept
{
    m_impl->status(s);
}

uint32_t header::contentLength() const noexcept
{
    return m_impl->contentLength();
}

void header::setContentLength(uint32_t length) noexcept
{
    return m_impl->setContentLength(length);
}

std::string header::toString() const noexcept
{
    return m_impl->toString();
}

const std::string &header::getContentType() const noexcept
{
    return m_impl->getContentType();
}

void header::setContentType(const std::string &type) noexcept
{
    m_impl->setContentType(type);
}

std::vector<uint8_t> header::toVector() const noexcept
{
    return m_impl->toVector();
}
