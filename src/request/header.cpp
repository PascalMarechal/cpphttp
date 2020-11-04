#include "header.h"
#include <sstream>

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
    std::string line;
    std::istringstream iss(data);
    while (getline(iss, line))
        if (line.empty())
            m_isReady = true;
    return "";
}
