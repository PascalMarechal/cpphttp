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
    head.read("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(Header, CompleteWindowsHeader)
{
    header head;
    head.read("GET /index HTTP/1.0\r\n\r\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(Header, IncompleteHeader)
{
    header head;
    head.read("GET /index HTTP/1.0");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, MissingRequestLine)
{
    header head;
    head.read("User-Agent: HTTPTool/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, IncompleteRequestLine)
{
    header head;
    head.read("GET HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, WrongOrderRequestLine)
{
    header head;
    head.read("GET HTTP/1.0 /hello\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, CompleteHeaderData)
{
    header head;
    head.read("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
    EXPECT_EQ(head.getMethod(), method::GET);
    EXPECT_EQ(head.getPath(), "/index");
    EXPECT_EQ(head.getVersion(), version::_1);
}

TEST(Header, ChunkedHeaderData)
{
    header head;
    head.read("GET /ind");
    head.read("ex HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
    EXPECT_EQ(head.getMethod(), method::GET);
    EXPECT_EQ(head.getPath(), "/index");
    EXPECT_EQ(head.getVersion(), version::_1);
}
