/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "requests.h"
#include "common/tools/data_reader.h"

std::string Requests::POST_REQUEST_HEADER = readFile("./data/headers/post_request.txt");
std::string Requests::GET_REQUEST_HEADER = readFile("./data/headers/get_request.txt");
std::string Requests::GET_REQUEST_HEADER_WITH_PARAM = readFile("./data/headers/get_request_with_param.txt");
std::string Requests::GET_REQUEST_HEADER_WITH_PARAM2 = readFile("./data/headers/get_request_with_param2.txt");
std::string Requests::GET_REQUEST_HEADER_WITH_UTF8_PARAM = readFile("./data/headers/get_request_with_UTF8_param.txt");
std::string Requests::POST_REQUEST_HEADER_WITH_BODY_LENGTH = readFile("./data/headers/post_request_incorrect_length.txt");
std::string Requests::POST_REQUEST_BODY = readFile("./data/body/post_request.txt");
std::string Requests::POST_REQUEST_BODY_WITH_UTF8 = readFile("./data/body/post_request_with_UTF8.txt");
std::string Requests::POST_REQUEST_HEADER_WITH_BIG_BODY_LENGTH = readFile("./data/headers/post_request_with_big_body.txt");
uint32_t Requests::EXPECTED_POST_BODY_SIZE = 32;

std::unique_ptr<cpphttp::request::request> getCorrectPostRequest()
{
    auto req = std::make_unique<cpphttp::request::request>();
    req->setHeader(Requests::POST_REQUEST_HEADER);
    req->setBody(Requests::POST_REQUEST_BODY);
    return req;
}

std::unique_ptr<cpphttp::request::request> getCorrectGetRequest()
{
    auto req = std::make_unique<cpphttp::request::request>();
    req->setHeader(Requests::GET_REQUEST_HEADER);
    return req;
}

std::unique_ptr<cpphttp::request::request> getGetRequestWithParam()
{
    auto req = std::make_unique<cpphttp::request::request>();
    req->setHeader(Requests::GET_REQUEST_HEADER_WITH_PARAM);
    return req;
}

std::unique_ptr<cpphttp::request::request> getGetRequestWithTailingParams()
{
    auto req = std::make_unique<cpphttp::request::request>();
    req->setHeader(Requests::GET_REQUEST_HEADER_WITH_PARAM2);
    return req;
}

std::unique_ptr<cpphttp::request::request> Requests::PostRequest = getCorrectPostRequest();
std::unique_ptr<cpphttp::request::request> Requests::GetRequest = getCorrectGetRequest();
std::unique_ptr<cpphttp::request::request> Requests::GetRequestWithParam = getGetRequestWithParam();
std::unique_ptr<cpphttp::request::request> Requests::GetRequestWithTailingParams = getGetRequestWithTailingParams();
