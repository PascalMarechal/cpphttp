#include "request/header.h"
#include <gtest/gtest.h>

using namespace cpphttp::request;

TEST(Header, EmptyHeader)
{
    header head;
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, CompleteHeader)
{
    header head;
    head.parse("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(Header, IncompleteHeader)
{
    header head;
    head.parse("GET /index HTTP/1.0");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, MissingRequestLine)
{
    header head;
    head.parse("User-Agent: HTTPTool/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, IncompleteRequestLine)
{
    header head;
    head.parse("GET HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, WrongOrderRequestLine)
{
    header head;
    head.parse("GET HTTP/1.0 /hello\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, CompleteHeaderData)
{
    header head;
    head.parse("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
    EXPECT_EQ(head.getMethod(), method::GET);
    EXPECT_EQ(head.getPath(), "/index");
    EXPECT_EQ(head.getVersion(), version::_1);
}
