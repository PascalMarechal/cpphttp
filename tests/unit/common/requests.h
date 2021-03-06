/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include <string>
#include "request/request.h"

class Requests
{
public:
    static std::string POST_REQUEST_HEADER;
    static std::string GET_REQUEST_HEADER;
    static std::string POST_REQUEST_HEADER_WITH_BODY_LENGTH;
    static std::string POST_REQUEST_HEADER_WITH_BIG_BODY_LENGTH;
    static std::string POST_REQUEST_BODY;
    static std::string POST_REQUEST_BODY_WITH_UTF8;
    static std::string GET_REQUEST_HEADER_WITH_PARAM;
    static std::string GET_REQUEST_HEADER_WITH_PARAM2;
    static std::string GET_REQUEST_HEADER_WITH_UTF8_PARAM;
    static uint32_t EXPECTED_POST_BODY_SIZE;

    static std::unique_ptr<cpphttp::request::request> PostRequest;
    static std::unique_ptr<cpphttp::request::request> GetRequestWithParam;
    static std::unique_ptr<cpphttp::request::request> GetRequest;
    static std::unique_ptr<cpphttp::request::request> GetRequestWithTailingParams;

    static std::unique_ptr<cpphttp::request::request> GetRequestFromPath(const std::string& path);
};
