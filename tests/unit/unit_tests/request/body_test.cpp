/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include "request/body.h"

using namespace cpphttp::request;

TEST(Body, Empty_body)
{
    body body("");
    EXPECT_EQ(body.getSize(), 0);
}

TEST(Body, Filled_body)
{
    body body("Something");
    EXPECT_EQ(body.getSize(), 9);
}

TEST(Body, Equality_operators)
{
    body aBody("Something");
    body sameBody("Something");
    EXPECT_TRUE(aBody == sameBody);
    body differentBody("Homething");
    EXPECT_TRUE(aBody != differentBody);
}
