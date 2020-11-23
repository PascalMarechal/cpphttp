#include "requests.h"
#include "common/tools/data_reader.h"

std::string Requests::PostRequestHeader = readFile("./data/headers/post_request.txt");
std::string Requests::GetRequestHeader = readFile("./data/headers/get_request.txt");
std::string Requests::GetRequestHeaderWithParam = readFile("./data/headers/get_request_with_param.txt");
std::string Requests::GetRequestHeaderWithParam2 = readFile("./data/headers/get_request_with_param2.txt");
std::string Requests::PostRequestHeaderWithIncorrectLength = readFile("./data/headers/post_request_incorrect_length.txt");
std::string Requests::PostRequestBody = readFile("./data/body/post_request.txt");
std::string Requests::PostRequestHeaderWithBigBodyLength = readFile("./data/headers/post_request_with_big_body.txt");
uint32_t Requests::ExpectedPostBodySize = 32;

std::unique_ptr<cpphttp::request::request> getCorrectPostRequest()
{
    auto req = std::make_unique<cpphttp::request::request>();
    req->setHeader(Requests::PostRequestHeader);
    req->setBody(Requests::PostRequestBody);
    return req;
}

std::unique_ptr<cpphttp::request::request> getCorrectGetRequest()
{
    auto req = std::make_unique<cpphttp::request::request>();
    req->setHeader(Requests::GetRequestHeader);
    return req;
}

std::unique_ptr<cpphttp::request::request> getGetRequestWithParam()
{
    auto req = std::make_unique<cpphttp::request::request>();
    req->setHeader(Requests::GetRequestHeaderWithParam);
    return req;
}

std::unique_ptr<cpphttp::request::request> getGetRequestWithTailingParams()
{
    auto req = std::make_unique<cpphttp::request::request>();
    req->setHeader(Requests::GetRequestHeaderWithParam2);
    return req;
}

std::unique_ptr<cpphttp::request::request> Requests::PostRequest = getCorrectPostRequest();
std::unique_ptr<cpphttp::request::request> Requests::GetRequest = getCorrectGetRequest();
std::unique_ptr<cpphttp::request::request> Requests::GetRequestWithParam = getGetRequestWithParam();
std::unique_ptr<cpphttp::request::request> Requests::GetRequestWithTailingParams = getGetRequestWithTailingParams();
