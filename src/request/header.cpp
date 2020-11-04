#include "header.h"
#include "../tools/string.h"
#include <sstream>
#include <unordered_map>

using namespace cpphttp::request;

header::header() : m_ready(false), m_method(method::UNKNOWN), m_version(version::UNKNOWN)
{
}

const std::unordered_map<std::string, method> methodMapping =
    {{"GET", method::GET}, {"POST", method::POST}, {"DELETE", method::DELETE}, {"PUT", method::PUT}, {"HEAD", method::HEAD}, {"PATCH", method::PATCH}};

inline void header::parseMethodValue(const std::string &value) noexcept
{
    auto method = methodMapping.find(value);
    if (method == methodMapping.end())
        return;
    m_method = method->second;
}

const std::unordered_map<std::string, version> versionMapping =
    {{"1.0", version::_1}, {"1.1", version::_1_1}};

inline void header::parseVersionValue(const std::string &value) noexcept
{
    auto versionInfo = split(value, '/');
    if (versionInfo.size() != 2 || versionInfo[0] != "HTTP")
        return;
    auto version = versionMapping.find(versionInfo[1]);
    if (version == versionMapping.end())
        return;
    m_version = version->second;
}

inline void header::parseRequestLine(const std::string &line) noexcept
{
    if (getMethod() != method::UNKNOWN)
        return;

    auto values = split(line, ' ');
    if (values.size() != 3)
        return;

    parseVersionValue(values[2]);
    setPath(values[1]);
    parseMethodValue(values[0]);
}

inline void header::handleHeaderLine(const std::string &line) noexcept
{
    auto colonPosition = line.find(':');
    if (colonPosition == std::string::npos)
        return parseRequestLine(line);
}

std::string header::parse(const std::string &data) noexcept
{
    std::string line;
    std::istringstream iss(data);
    while (getline(iss, line))
        if (line.empty())
            m_ready = m_method != method::UNKNOWN && m_version != version::UNKNOWN && m_path.size() > 0;
        else
            handleHeaderLine(line);
    return "";
}

bool header::isReady() noexcept
{
    return m_ready;
}

method header::getMethod() noexcept
{
    return m_method;
}

version header::getVersion() noexcept
{
    return m_version;
}

std::string header::getPath() noexcept
{
    return m_path;
}

void header::setPath(std::string path) noexcept
{
    m_path = path;
}
