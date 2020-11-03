#include "request.h"

using namespace cpphttp;

request::request() : m_isReady(false)
{
}

bool request::isReady()
{
    return m_isReady;
}

void request::fill(const std::string &data)
{
    m_isReady = true;
}
