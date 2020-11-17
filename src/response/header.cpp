#include "header.h"
#include <unordered_map>
#include <sstream>

using namespace cpphttp;
using namespace response;

header::header() : m_status(status::_200), m_contentLength(0)
{
}

request::version header::version() const noexcept
{
    return request::version::_1_1;
}

status header::status() const noexcept
{
    return m_status;
}

void header::setStatus(response::status status) noexcept
{
    m_status = status;
}

uint32_t header::contentLength() const noexcept
{
    return m_contentLength;
}

void header::setContentLength(uint32_t length) noexcept
{
    m_contentLength = length;
}

const std::unordered_map<status, std::string>
    statusMapping =
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
         {status::_405, "405 Method Not Allowed"}};

std::string header::toString() const noexcept
{
    std::ostringstream result;

    auto statusText = statusMapping.find(m_status);
    result << "HTTP/1.1 " << statusText->second
           << "\r\nContent-Length: " << m_contentLength << "\r\n\r\n";
    return result.str();
}
