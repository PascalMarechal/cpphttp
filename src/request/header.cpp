#include "header.h"
#include "../tools/string.h"
#include <sstream>
#include <unordered_map>

using namespace cpphttp::request;

header::header() : m_isReady(false), m_method(method::UNKNOWN)
{
}

bool header::isReady()
{
    return m_isReady;
}

const std::unordered_map<std::string, method> methodMapping =
    {{"GET", method::GET}, {"POST", method::POST}, {"DELETE", method::DELETE}, {"PUT", method::PUT}, {"HEAD", method::HEAD}, {"PATCH", method::PATCH}};

void header::parseRequestLine(const std::string &line)
{
    if (m_method != method::UNKNOWN)
        return;

    auto values = split(line, ' ');

    auto method = methodMapping.find(values[0]);
    if (method == methodMapping.end())
        return;
    m_method = method->second;
}

void header::handleHeaderLine(const std::string &line)
{
    auto colonPosition = line.find(':');
    if (colonPosition == std::string::npos)
        return parseRequestLine(line);
}

std::string header::fill(const std::string &data)
{
    std::string line;
    std::istringstream iss(data);
    while (getline(iss, line))
        if (line.empty())
            m_isReady = m_method != method::UNKNOWN;
        else
            handleHeaderLine(line);
    return "";
}
