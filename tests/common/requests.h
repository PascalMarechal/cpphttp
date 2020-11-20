#pragma once

#include <string>

class Requests
{
public:
    static std::string PostRequestHeader;
    static std::string GetRequestHeader;
    static std::string PostRequestHeaderWithIncorrectLength;
    static std::string PostRequestBody;
    static std::string GetRequestHeaderWithParam;
    static std::string GetRequestHeaderWithParam2;
    static uint32_t ExpectedPostBodySize;
};
