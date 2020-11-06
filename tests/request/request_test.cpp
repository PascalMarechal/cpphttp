#include "request/request.h"
#include <gtest/gtest.h>
#include "../tools/data_reader.h"
#include "tools/string.h"
#include <unistd.h>

using namespace cpphttp::request;

std::string getRequest = readFile("./data/get_request.txt");
std::string incompletePostRequest = readFile("./data/incomplete_post_request.txt");
std::string twoPostRequest = readFile("./data/two_post_requests.txt");
extern std::string postRequest;
extern std::string jsonPostRequest;

TEST(Request, EmptyRequest)
{
    request req;
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, CompleteRequest)
{
    request req;
    req.read("GET /index HTTP/1.1\n\n");
    EXPECT_EQ(req.isReady(), true);
}

TEST(Request, IncompleteRequest)
{
    request req;
    req.read("GET /index HTTP/1.1");
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, IncompletePost)
{
    request req;
    req.read(incompletePostRequest);
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, FullPostRequest)
{
    request req;
    req.read(postRequest);
    EXPECT_EQ(req.isReady(), true);
}

TEST(Request, FullWindowsPostRequest)
{
    request req;
    req.read("POST /path/script.cgi HTTP/1.0\r\nContent-Length: 32\r\n\r\nhome=Cosby&favorite+flavor=flies");
    EXPECT_EQ(req.isReady(), true);
}

TEST(Request, TwoFullPostRequest)
{
    request req;
    auto remainder = req.read(twoPostRequest);
    EXPECT_EQ(req.isReady(), true);
    request req2;
    req2.read(remainder);
    EXPECT_EQ(req2.isReady(), true);
}

TEST(Request, SplittedJSON)
{
    request req;
    auto splittedRequest = split(jsonPostRequest, '[');
    for(const auto& value : splittedRequest)
        req.read(value);
    EXPECT_EQ(req.isReady(), true);
}
