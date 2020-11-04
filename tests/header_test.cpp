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
    head.fill("GET /index HTTP/1.1\n\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(Header, IncompleteHeader)
{
    header head;
    head.fill("GET /index HTTP/1.1");
    EXPECT_EQ(head.isReady(), false);
}
