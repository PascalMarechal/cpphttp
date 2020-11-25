#include "header.h"
#include "internal/tools/string.h"
#include <sstream>
#include <unordered_map>
#include <functional>

using namespace cpphttp::request;
using namespace cpphttp::internal;

class header::impl
{
public:
    impl(const std::string_view &data) : m_ready(false), m_method(method::UNKNOWN), m_version(version::UNKNOWN), m_expectedBodysize(0)
    {
        parse(data);
    }

    inline bool isReady() const noexcept
    {
        return m_ready;
    }

    inline method getMethod() const noexcept
    {
        return m_method;
    }

    inline version getVersion() const noexcept
    {
        return m_version;
    }
    inline const std::string &getPath() const noexcept
    {
        return m_path;
    }

    inline const std::string &getGetParams() const noexcept
    {
        return m_getParams;
    }

    inline uint32_t getExpectedBodySize() const noexcept
    {
        return m_expectedBodysize;
    }

    inline void setPath(const std::string_view &path) noexcept
    {
        if (!extractGetPath(path))
            m_path = path;
    }

    inline friend bool operator==(const impl &lhs, const impl &rhs) noexcept
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
    inline void parseMethodValue(const std::string_view &value) noexcept
    {
        auto method = methodMapping.find(value);
        if (method == methodMapping.end())
            return;
        m_method = method->second;
    }

    inline void parseVersionValue(const std::string_view &value) noexcept
    {
        auto versionInfo = split(value, "/");
        if (versionInfo.size() != 2 || versionInfo[0] != "HTTP")
            return;

        auto version = versionMapping.find(versionInfo[1]);
        if (version == versionMapping.end())
            return;
        m_version = version->second;
    }

    inline void parseRequestLine(const std::string_view &line) noexcept
    {
        if (getMethod() != method::UNKNOWN)
            return;

        auto values = split(line, " ");
        if (values.size() != 3)
            return;

        parseVersionValue(values[2]);
        parseMethodValue(values[0]);
        setPath(values[1]);
    }

    inline void handleHeaderLine(const std::string_view &line) noexcept
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
    inline void parse(const std::string_view &data) noexcept
    {
        auto lines = split(data, "\r\n");
        for (auto line : lines)
            handleHeaderLine(line);

        m_ready = m_method != method::UNKNOWN && m_version != version::UNKNOWN && m_path.size() > 0;
    }

    inline bool extractGetPath(const std::string_view &path) noexcept
    {
        if (m_method != method::GET)
            return false;

        auto splittedPath = split(path, "?");
        m_path = splittedPath[0];
        for (auto i = 1; i < splittedPath.size(); ++i)
            m_getParams += splittedPath[i];

        return true;
    }

    bool m_ready;
    method m_method;
    version m_version;
    uint32_t m_expectedBodysize;
    std::string m_path;
    std::string m_getParams;

    const inline static std::unordered_map<std::string_view, method> methodMapping =
        {{"GET", method::GET}, {"POST", method::POST}, {"DELETE", method::DELETE}, {"PUT", method::PUT}, {"HEAD", method::HEAD}, {"PATCH", method::PATCH}};

    const static std::unordered_map<std::string_view, std::function<void(header::impl *, std::string &&)>> fillFunctions;
    const inline static std::unordered_map<std::string_view, version> versionMapping =
        {{"1.0", version::_1}, {"1.1", version::_1_1}};
};

const std::unordered_map<std::string_view, std::function<void(header::impl *, std::string &&)>> header::impl::fillFunctions =
    {{"Content-Length", [](header::impl *head, std::string &&s) {
          head->setExpectedBodySize(std::stoul(s));
      }}};

header::header(const std::string_view &data) : m_impl(std::make_unique<impl>(data))
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

const std::string &header::getGetParams() const noexcept
{
    return m_impl->getGetParams();
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
