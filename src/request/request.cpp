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
                return m_header->getExpectedBodySize() == 0;
            else
                return m_header->getExpectedBodySize() == m_body->getSize();
        }
        return false;
    }

    inline void setHeader(const std::string &data) noexcept
    {
        m_header = std::make_unique<cpphttp::request::header>(data);
        extractTailingParameters();
    }

    inline void setBody(const std::string &data) noexcept
    {
        m_body = std::make_unique<body>(data);
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

    inline void set(std::string name, std::string value) noexcept
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

private:
    void extractTailingParameters()
    {
        auto params = tools::split(m_header->getGetParams(), "&");
        for (const auto &param : params)
        {
            auto paramKeyValue = tools::split(param, "=");
            if (paramKeyValue.size() != 2)
                continue;
            m_param_values[std::string(paramKeyValue[0])] = paramKeyValue[1];
        }
    }

    std::unique_ptr<cpphttp::request::header> m_header;
    std::unique_ptr<body> m_body;
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

void request::setHeader(const std::string &data) noexcept
{
    m_impl->setHeader(data);
}

void request::setBody(const std::string &data) noexcept
{
    m_impl->setBody(data);
}

void request::set(std::string name, std::string value) noexcept
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
