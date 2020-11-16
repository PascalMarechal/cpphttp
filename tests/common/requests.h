#pragma once

#include <string>

class Requests
{
public:
    static std::string PostRequestHeader;
    static std::string GetRequestHeader;
    static std::string PostRequestHeaderWithIncorrectLength;
    static std::string PostRequestBody;
    static uint32_t ExpectedPostBodySize;
};
