#include "request/header.h"
#include <gtest/gtest.h>
#include "tools/string.h"
#include "common/requests.h"

using namespace cpphttp::request;
using namespace cpphttp::tools;

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
    header head(Requests::PostRequestHeader);
    EXPECT_EQ(head.getExpectedBodySize(), 32);
}

TEST(Header, IncorrectBodySizeValue)
{
    header head(Requests::PostRequestHeaderWithIncorrectLength);
    EXPECT_EQ(head.getExpectedBodySize(), 0);
}

TEST(Header, EqualityOperators)
{
    header head("GET /index HTTP/1.0\n\n");
    header head2("GET /index HTTP/1.0\n\n");
    EXPECT_TRUE(head == head2);
    header head3("POST /index HTTP/1.0\n\n");
    EXPECT_TRUE(head != head3);
    header head4("GET /path42 HTTP/1.0\n\n");
    EXPECT_TRUE(head != head4);
    header head5("GET /index HTTP/1.1\n\n");
    EXPECT_TRUE(head != head5);
}
