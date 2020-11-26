/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "request/header.h"
#include <gtest/gtest.h>
#include "common/requests.h"

using namespace cpphttp::request;

TEST(RequestHeader, Empty_header)
{
    header head("");
    EXPECT_EQ(head.isReady(), false);
}

TEST(RequestHeader, Complete_header)
{
    header head("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(RequestHeader, Complete_CRLF_header)
{
    header head("GET /index HTTP/1.0\r\n\r\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(RequestHeader, Missing_RL)
{
    header head("User-Agent: HTTPTool/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(RequestHeader, Incomplete_request_line)
{
    header head("GET HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(RequestHeader, Wrong_order_request_line)
{
    header head("GET HTTP/1.0 /hello\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(RequestHeader, Complete_header_data)
{
    header head("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
    EXPECT_EQ(head.getMethod(), method::GET);
    EXPECT_EQ(head.getPath(), "/index");
    EXPECT_EQ(head.getVersion(), version::_1);
}

TEST(RequestHeader, Read_expected_body_size)
{
    header head(Requests::POST_REQUEST_HEADER);
    EXPECT_EQ(head.getExpectedBodySize(), 32);
}

TEST(RequestHeader, Incorrect_body_size_value)
{
    header head(Requests::POST_REQUEST_HEADER_WITH_BODY_LENGTH);
    EXPECT_EQ(head.getExpectedBodySize(), 0);
}

TEST(RequestHeader, Equality_operators)
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

TEST(RequestHeader, Should_split_path_with_get_params)
{
    header head("GET /index?id=21&page=3 HTTP/1.0\n\n");
    EXPECT_EQ(head.getPath(), "/index");
    EXPECT_EQ(head.getGetParams(), "id=21&page=3");

    header head2("POST /index?id=21&page=3 HTTP/1.0\n\n");
    EXPECT_EQ(head2.getPath(), "/index?id=21&page=3");
    EXPECT_EQ(head2.getGetParams(), "");

    header head3("PUT /index?id=21&page=3 HTTP/1.0\n\n");
    EXPECT_EQ(head3.getPath(), "/index?id=21&page=3");
    EXPECT_EQ(head3.getGetParams(), "");
}
