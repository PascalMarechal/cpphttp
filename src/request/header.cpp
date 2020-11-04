#include "header.h"

using namespace cpphttp;

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
