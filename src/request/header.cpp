#include "header.h"

using namespace cpphttp::request;

header::header() : m_isReady(false)
{
}

bool header::isReady()
{
    return m_isReady;
}

std::string header::fill(const std::string &data)
{
    m_isReady = true;
    return "";
}
