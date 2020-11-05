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

void request::read(const std::string &data)
{
    m_header.read(data);
}
