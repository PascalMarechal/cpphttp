#include "header.h"
#include "internal/tools/string.h"
#include <sstream>
#include <unordered_map>
#include <functional>

using namespace cpphttp::request;
using namespace cpphttp::tools;

class header::impl
{
public:
    impl(const std::string &data) : m_ready(false), m_method(method::UNKNOWN), m_version(version::UNKNOWN), m_expectedBodysize(0)
    {
        parse(data);
    }

    bool isReady() const noexcept
    {
        return m_ready;
    }

    method getMethod() const noexcept
    {
        return m_method;
    }

    version getVersion() const noexcept
    {
        return m_version;
    }
    const std::string &getPath() const noexcept
    {
        return m_path;
    }

    uint32_t getExpectedBodySize() const noexcept
    {
        return m_expectedBodysize;
    }

    void setPath(std::string_view path) noexcept
    {
        m_path = path;
    }

    friend bool operator==(const impl &lhs, const impl &rhs) noexcept
    {
        return lhs.m_ready == rhs.m_ready &&
               lhs.m_method == rhs.m_method &&
               lhs.m_version == rhs.m_version &&
               lhs.m_expectedBodysize == rhs.m_expectedBodysize &&
               lhs.m_path == rhs.m_path;
    }

    inline void setExpectedBodySize(uint32_t size) noexcept
    {
        m_expectedBodysize = size;
    }

private:
    inline void parseMethodValue(std::string_view value) noexcept
    {
        auto method = methodMapping.find(value);
        if (method == methodMapping.end())
            return;
        m_method = method->second;
    }

    inline void parseVersionValue(std::string_view value) noexcept
    {
        auto versionInfo = split(value, "/");
        if (versionInfo.size() != 2 || versionInfo[0] != "HTTP")
            return;

        auto version = versionMapping.find(versionInfo[1]);
        if (version == versionMapping.end())
            return;
        m_version = version->second;
    }

    inline void parseRequestLine(std::string_view line) noexcept
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

    inline void handleHeaderLine(std::string_view line) noexcept
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
    inline void parse(const std::string &data) noexcept
    {
        auto lines = split(data, "\r\n");
        for (auto line : lines)
            handleHeaderLine(line);

        m_ready = m_method != method::UNKNOWN && m_version != version::UNKNOWN && m_path.size() > 0;
    }

    bool m_ready;
    method m_method;
    version m_version;
    uint32_t m_expectedBodysize;
    std::string m_path;

    const inline static std::unordered_map<std::string_view, method> methodMapping =
        {{"GET", method::GET}, {"POST", method::POST}, {"DELETE", method::DELETE}, {"PUT", method::PUT}, {"HEAD", method::HEAD}, {"PATCH", method::PATCH}};

    const inline static std::unordered_map<std::string_view, std::function<void(header::impl *, std::string &&)>> fillFunctions =
        {{"Content-Length", [](header::impl *head, std::string &&s) {
              head->setExpectedBodySize(std::stoul(s));
          }}};

    const inline static std::unordered_map<std::string_view, version> versionMapping =
        {{"1.0", version::_1}, {"1.1", version::_1_1}};
};

header::header(const std::string &data) : m_impl(std::make_unique<impl>(data))
{
}

header::~header() {}

bool header::isReady() const noexcept
{
    return m_impl->isReady();
}

method header::getMethod() const noexcept
{
    return m_impl->getMethod();
}

version header::getVersion() const noexcept
{
    return m_impl->getVersion();
}

const std::string &header::getPath() const noexcept
{
    return m_impl->getPath();
}

uint32_t header::getExpectedBodySize() const noexcept
{
    return m_impl->getExpectedBodySize();
}

namespace cpphttp
{
    namespace request
    {
        bool operator==(const header &lhs, const header &rhs) noexcept
        {
            return *lhs.m_impl == *rhs.m_impl;
        }

        bool operator!=(const header &lhs, const header &rhs) noexcept
        {
            return !(lhs == rhs);
        }
    } // namespace request
} // namespace cpphttp
