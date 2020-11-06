#include "request.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <functional>

using namespace cpphttp::request;

bool request::isReady()
{
    return m_header.isReady() && m_body.getSize() == m_header.getExpectedBodySize();
}

std::string request::appendToBody(const std::string &data) noexcept
{
    auto endOfDataToAppend = std::min(data.size(), m_header.getExpectedBodySize() - m_body.getSize());

    if (endOfDataToAppend == 0)
        return data;

    auto dataToAppend = data.substr(0, endOfDataToAppend);
    m_body.appendData(dataToAppend);
    return data.substr(endOfDataToAppend);
}

std::string request::read(const std::string &data)
{
    auto remainder = m_header.read(data);
    return appendToBody(remainder);
}
