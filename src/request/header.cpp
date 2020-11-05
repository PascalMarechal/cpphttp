#include "header.h"
#include "../tools/string.h"
#include <sstream>
#include <unordered_map>
#include <functional>

using namespace cpphttp::request;

header::header() : m_ready(false), m_headerReadComplete(false), m_method(method::UNKNOWN), m_version(version::UNKNOWN), m_expectedBodysize(0)
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

const std::unordered_map<std::string, std::function<void(header *, const std::string &)>> fillFunctions =
    {{"Content-Length", [](header *head, const std::string &s) {
          head->setExpectedBodySize(std::stoul(s));
      }}};

inline void header::handleHeaderLine(const std::string &line) noexcept
{
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
        toCall->second(this, argument);
    }
    catch (...)
    {
    }
}

void header::parse() noexcept
{
    std::string line;
    std::istringstream iss(m_rawData);
    while (getline(iss, line))
        handleHeaderLine(line);

    m_ready = m_method != method::UNKNOWN && m_version != version::UNKNOWN && m_path.size() > 0;
}

std::size_t header::findEndOfHeaderData(const std::string &data) noexcept
{
    // Linux test
    auto endOfHeader = data.find("\n\n");
    // Windows test
    if (endOfHeader == std::string::npos)
        endOfHeader = data.find("\r\n\r\n");

    return endOfHeader;
}

void header::appendRawData(const std::string &data, std::size_t to) noexcept
{
    if (to == std::string::npos)
        m_rawData += data;
    else
        m_rawData += data.substr(0, to);
}

std::string header::read(const std::string &data) noexcept
{
    if (m_headerReadComplete)
        return data;

    auto endOfHeader = header::findEndOfHeaderData(data);
    appendRawData(data, endOfHeader);

    m_headerReadComplete = endOfHeader != std::string::npos;
    if (m_headerReadComplete)
        parse();
    else
        return "";

    return data.substr(endOfHeader);
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

uint32_t header::getExpectedBodySize() noexcept
{
    return m_expectedBodysize;
}

void header::setPath(std::string path) noexcept
{
    m_path = path;
}

void header::setExpectedBodySize(uint32_t size) noexcept
{
    m_expectedBodysize = size;
}
