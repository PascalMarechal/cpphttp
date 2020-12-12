/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
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

    inline cpphttp::request::method method() const noexcept
    {
        return m_method;
    }

    inline cpphttp::request::version version() const noexcept
    {
        return m_version;
    }
    inline const std::string &path() const noexcept
    {
        return m_path;
    }

    inline const std::string &getParams() const noexcept
    {
        return m_getParams;
    }

    inline uint32_t expectedBodySize() const noexcept
    {
        return m_expectedBodysize;
    }

    inline void setPath(const std::string_view &path) noexcept
    {
        if (!extractpath(path))
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
        auto method = METHOD_MAPPING.find(value);
        if (method == METHOD_MAPPING.end())
            return;
        m_method = method->second;
    }

    inline void parseVersionValue(const std::string_view &value) noexcept
    {
        auto versionInfo = split(value, "/");
        if (versionInfo.size() != 2 || versionInfo[0] != "HTTP")
            return;

        auto version = VERSION_MAPPING.find(versionInfo[1]);
        if (version == VERSION_MAPPING.end())
            return;
        m_version = version->second;
    }

    inline void parseRequestLine(const std::string_view &line) noexcept
    {
        if (method() != method::UNKNOWN)
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

        auto toCall = FILL_FUNCTIONS.find(line.substr(0, colonPosition));
        if (toCall == FILL_FUNCTIONS.end())
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

    inline bool extractpath(const std::string_view &path) noexcept
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
    cpphttp::request::method m_method;
    cpphttp::request::version m_version;
    uint32_t m_expectedBodysize;
    std::string m_path;
    std::string m_getParams;

    const inline static std::unordered_map<std::string_view, cpphttp::request::method> METHOD_MAPPING =
        {{"GET", method::GET}, {"POST", method::POST}, {"DELETE", method::DELETE}, {"PUT", method::PUT}, {"HEAD", method::HEAD}, {"PATCH", method::PATCH}};

    const static std::unordered_map<std::string_view, std::function<void(header::impl *, std::string &&)>> FILL_FUNCTIONS;
    const inline static std::unordered_map<std::string_view, cpphttp::request::version> VERSION_MAPPING =
        {{"1.0", version::_1}, {"1.1", version::_1_1}};
};

const std::unordered_map<std::string_view, std::function<void(header::impl *, std::string &&)>> header::impl::FILL_FUNCTIONS =
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

cpphttp::request::method header::method() const noexcept
{
    return m_impl->method();
}

cpphttp::request::version header::version() const noexcept
{
    return m_impl->version();
}

const std::string &header::path() const noexcept
{
    return m_impl->path();
}

const std::string &header::getParams() const noexcept
{
    return m_impl->getParams();
}

uint32_t header::expectedBodySize() const noexcept
{
    return m_impl->expectedBodySize();
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
