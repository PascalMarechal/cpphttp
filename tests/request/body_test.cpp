#include <gtest/gtest.h>
#include "request/body.h"

using namespace cpphttp::request;

TEST(Body, EmptyBody)
{
    body body;
    EXPECT_EQ(body.getSize(), 0);
}

TEST(Body, FilledBody)
{
    body body;
    body.appendData("Something");
    EXPECT_EQ(body.getSize(), 9);
}

TEST(Body, ChunkedBody)
{
    body body;
    body.appendData("Something");
    body.appendData("Something");
    EXPECT_EQ(body.getSize(), 9*2);
}

