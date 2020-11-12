#include "header.h"
#include "../tools/string.h"
#include <sstream>
#include <unordered_map>
#include <functional>

using namespace cpphttp::request;
using namespace cpphttp::tools;

header::header(const std::string &data) : m_ready(false), m_method(method::UNKNOWN), m_version(version::UNKNOWN), m_expectedBodysize(0)
{
    parse(data);
}

const std::unordered_map<std::string_view, method> methodMapping =
    {{"GET", method::GET}, {"POST", method::POST}, {"DELETE", method::DELETE}, {"PUT", method::PUT}, {"HEAD", method::HEAD}, {"PATCH", method::PATCH}};

inline void header::parseMethodValue(std::string_view value) noexcept
{
    auto method = methodMapping.find(value);
    if (method == methodMapping.end())
        return;
    m_method = method->second;
}

const std::unordered_map<std::string_view, version> versionMapping =
    {{"1.0", version::_1}, {"1.1", version::_1_1}};

inline void header::parseVersionValue(std::string_view value) noexcept
{
    auto versionInfo = split(value, "/");
    if (versionInfo.size() != 2 || versionInfo[0] != "HTTP")
        return;

    auto version = versionMapping.find(versionInfo[1]);
    if (version == versionMapping.end())
        return;
    m_version = version->second;
}

inline void header::parseRequestLine(std::string_view line) noexcept
{
    if (getMethod() != method::UNKNOWN)
        return;

    auto values = split(line, " ");
    if (values.size() != 3)
        return;

    parseVersionValue(values[2]);
    setPath(values[1]);
    parseMethodValue(values[0]);
}

const std::unordered_map<std::string_view, std::function<void(header *, std::string &&)>> fillFunctions =
    {{"Content-Length", [](header *head, std::string &&s) {
          head->setExpectedBodySize(std::stoul(s));
      }}};

inline void header::handleHeaderLine(std::string_view line) noexcept
{
    if (!line.size())
        return;

    auto colonPosition = line.find(':');
    if (colonPosition == std::string::npos)
        return parseRequestLine(line);

    auto toCall = fillFunctions.find(line.substr(0, colonPosition));
    if (toCall == fillFunctions.end())
        return;

    auto argument = line.substr(colonPosition + 1);
    if (!argument.size())
        return;

    try
    {
        toCall->second(this, std::string(argument));
    }
    catch (...)
    {
    }
}

void header::parse(const std::string &data) noexcept
{
    auto lines = split(data, "\r\n");
    for (auto line : lines)
        handleHeaderLine(line);

    m_ready = m_method != method::UNKNOWN && m_version != version::UNKNOWN && m_path.size() > 0;
}

bool header::isReady() const noexcept
{
    return m_ready;
}

method header::getMethod() const noexcept
{
    return m_method;
}

version header::getVersion() const noexcept
{
    return m_version;
}

const std::string &header::getPath() const noexcept
{
    return m_path;
}

uint32_t header::getExpectedBodySize() const noexcept
{
    return m_expectedBodysize;
}

void header::setPath(std::string_view path) noexcept
{
    m_path = path;
}

void header::setExpectedBodySize(uint32_t size) noexcept
{
    m_expectedBodysize = size;
}

namespace cpphttp
{
    namespace request
    {
        bool operator==(const header &lhs, const header &rhs) noexcept
        {
            return lhs.m_ready == rhs.m_ready &&
                   lhs.m_method == rhs.m_method &&
                   lhs.m_version == rhs.m_version &&
                   lhs.m_expectedBodysize == rhs.m_expectedBodysize &&
                   lhs.m_path == rhs.m_path;
        }

        bool operator!=(const header &lhs, const header &rhs) noexcept
        {
            return !(lhs == rhs);
        }
    } // namespace request
} // namespace cpphttp
