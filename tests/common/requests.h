#pragma once

#include <string>
#include "request/request.h"

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

    static std::unique_ptr<cpphttp::request::request> PostRequest;
    static std::unique_ptr<cpphttp::request::request> GetRequestWithParam;
    static std::unique_ptr<cpphttp::request::request> GetRequest;
};
