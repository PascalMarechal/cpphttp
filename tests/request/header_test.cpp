#include "request/header.h"
#include <gtest/gtest.h>
#include "../tools/data_reader.h"
#include "tools/string.h"

using namespace cpphttp::request;
using namespace cpphttp::tools;

std::string postRequest = readFile("./data/headers/post_request.txt");
std::string postRequestWithIncorrectLength = readFile("./data/headers/post_request_incorrect_length.txt");

TEST(Header, EmptyHeader)
{
    header head("");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, CompleteHeader)
{
    header head("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(Header, CompleteWindowsHeader)
{
    header head("GET /index HTTP/1.0\r\n\r\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(Header, MissingRequestLine)
{
    header head("User-Agent: HTTPTool/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, IncompleteRequestLine)
{
    header head("GET HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, WrongOrderRequestLine)
{
    header head("GET HTTP/1.0 /hello\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, CompleteHeaderData)
{
    header head("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
    EXPECT_EQ(head.getMethod(), method::GET);
    EXPECT_EQ(head.getPath(), "/index");
    EXPECT_EQ(head.getVersion(), version::_1);
}

TEST(Header, ReadExpectedBodySize)
{
    header head(postRequest);
    EXPECT_EQ(head.getExpectedBodySize(), 32);
}

TEST(Header, IncorrectBodySizeValue)
{
    header head(postRequestWithIncorrectLength);
    EXPECT_EQ(head.getExpectedBodySize(), 0);
}
