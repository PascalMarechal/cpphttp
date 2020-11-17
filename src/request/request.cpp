#include "request.h"

using namespace cpphttp::request;

class request::impl
{
public:
    bool isReady() const noexcept
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

    void setHeader(const std::string &data) noexcept
    {
        m_header = std::make_unique<cpphttp::request::header>(data);
    }
    void setBody(const std::string &data) noexcept
    {
        m_body = std::make_unique<body>(data);
    }
    const cpphttp::request::header &header() const
    {
        if (m_header == nullptr)
            throw std::invalid_argument("Header is NULL");
        return *(m_header.get());
    }

    friend bool operator==(const impl &lhs, const impl &rhs) noexcept
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

private:
    std::unique_ptr<cpphttp::request::header> m_header;
    std::unique_ptr<body> m_body;
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
