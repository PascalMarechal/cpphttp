#include "request.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <functional>

using namespace cpphttp;

const std::unordered_map<std::string, std::function<void(request *, const std::string &)>> fillFunctions =
    {{"Content-Length", [](request *req, const std::string &s) {
          req->setBodySize(std::atoi(s.c_str()));
      }}};

request::request() : m_isReady(false), m_bodySize(0)
{
}

bool request::isReady()
{
    return m_isReady;
}

void request::setBodySize(uint32_t size)
{
    m_bodySize = size;
}

void request::handleHeaderLine(const std::string &line)
{
    auto colonPosition = line.find(':');
    if (colonPosition == std::string::npos)
        return;

    auto toCall = fillFunctions.find(line.substr(0, colonPosition));
    if (toCall == fillFunctions.end())
        return;

    auto argument = line.substr(colonPosition + 1);
    if (argument.size())
        toCall->second(this, argument);
}

void request::fill(const std::string &data)
{
    std::string line;
    std::istringstream iss(data);
    while (getline(iss, line))
    {
        if (line.empty())
            m_isReady = m_body.size() == m_bodySize;
        else
            this->handleHeaderLine(line);
    }
}
