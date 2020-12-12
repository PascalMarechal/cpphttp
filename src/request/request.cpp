/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "request.h"
#include "internal/tools/string.h"
#include <unordered_map>

using namespace cpphttp::request;

class request::impl
{
public:
    inline bool isReady() const noexcept
    {
        if (m_header && m_header->isReady())
        {
            if (!m_body)
                return m_header->expectedBodySize() == 0;
            else
                return m_header->expectedBodySize() == m_body->size();
        }
        return false;
    }

    inline void header(const std::string_view &data) noexcept
    {
        m_header = std::make_unique<cpphttp::request::header>(data);
        extractParameters(m_header->getParams());
    }

    inline void body(const std::string_view &data) noexcept
    {
        m_body = std::make_unique<cpphttp::request::body>(data);
        if (m_header && m_header->method() == method::POST)
            extractParameters(data);
    }
    inline const cpphttp::request::header &header() const
    {
        if (m_header == nullptr)
            throw std::invalid_argument("Header is NULL");
        return *(m_header.get());
    }

    inline friend bool operator==(const impl &lhs, const impl &rhs) noexcept
    {
        if ((lhs.m_header == nullptr && rhs.m_header != nullptr) ||
            (lhs.m_header != nullptr && rhs.m_header == nullptr) ||
            (lhs.m_body == nullptr && rhs.m_body != nullptr) ||
            (lhs.m_body != nullptr && rhs.m_body == nullptr))
            return false;

        return (lhs.m_header == nullptr && rhs.m_header == nullptr ||
                *lhs.m_header == *rhs.m_header) &&
               (lhs.m_body == nullptr && rhs.m_body == nullptr ||
                *lhs.m_body == *rhs.m_body);
    }

    inline void set(const std::string &name, const std::string &value) noexcept
    {
        m_values[name] = value;
    }

    inline const std::string &get(const std::string &name) const noexcept
    {
        static const std::string empty = "";
        auto val = m_values.find(name);
        if (val != m_values.cend())
            return val->second;
        return empty;
    }

    inline bool has(const std::string &name) const noexcept
    {
        return m_values.find(name) != m_values.cend();
    }

    inline const std::string &getParam(const std::string &name) const noexcept
    {
        static const std::string empty = "";
        auto val = m_param_values.find(name);
        if (val != m_param_values.cend())
            return val->second;
        return empty;
    }

    inline void loadParamsFromUrl(std::string_view expectedPath) noexcept
    {
        auto splittedExpectedPath = internal::split(expectedPath, "/");
        auto splittedUrl = internal::split(m_header->path(), "/");
        auto pos = 0;
        for (const auto &val : splittedExpectedPath)
        {
            if (splittedUrl.size() == pos)
                return;

            if (val.size() > 1 && val[0] == ':')
                m_param_values[std::string(val.substr(1))] = splittedUrl[pos];

            ++pos;
        }
    }

private:
    void extractParameters(std::string_view from)
    {
        auto params = internal::split(from, "&");
        for (const auto &param : params)
        {
            auto paramKeyValue = internal::split(param, "=");
            if (paramKeyValue.size() < 2)
                continue;
            m_param_values[std::string(paramKeyValue[0])] = internal::uriDecode(paramKeyValue[1]);
        }
    }

    std::unique_ptr<cpphttp::request::header> m_header;
    std::unique_ptr<cpphttp::request::body> m_body;
    std::unordered_map<std::string, std::string> m_values;
    std::unordered_map<std::string, std::string> m_param_values;
};

request::request() : m_impl(std::make_unique<impl>()) {}

request::~request() {}

bool request::isReady() const noexcept
{
    return m_impl->isReady();
}

const header &request::header() const
{
    return m_impl->header();
}

void request::header(const std::string_view &data) noexcept
{
    m_impl->header(data);
}

void request::body(const std::string_view &data) noexcept
{
    m_impl->body(data);
}

void request::set(const std::string &name, const std::string &value) noexcept
{
    m_impl->set(name, value);
}

const std::string &request::get(const std::string &name) const noexcept
{
    return m_impl->get(name);
}

const std::string &request::getParam(const std::string &name) const noexcept
{
    return m_impl->getParam(name);
}

bool request::has(const std::string &name) const noexcept
{
    return m_impl->has(name);
}

void request::loadParamsFromUrl(const std::string_view &expectedPath) noexcept
{
    m_impl->loadParamsFromUrl(expectedPath);
}

namespace cpphttp
{
    namespace request
    {
        bool operator==(const request &lhs, const request &rhs) noexcept
        {
            return *lhs.m_impl == *rhs.m_impl;
        }

        bool operator!=(const request &lhs, const request &rhs) noexcept
        {
            return !(lhs == rhs);
        }
    } // namespace request
} // namespace cpphttp
