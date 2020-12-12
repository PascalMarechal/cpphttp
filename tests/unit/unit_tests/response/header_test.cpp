/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include "response/header.h"

using namespace cpphttp;
using namespace response;

TEST(ResponseHeader, Should_beV1_1_by_default)
{
    header head;
    EXPECT_EQ(head.version(), request::version::_1_1);
}

TEST(ResponseHeader, Status_should_be_200_at_start)
{
    header head;
    EXPECT_EQ(head.status(), status::_200);
}

TEST(ResponseHeader, Status_can_be_changed)
{
    header head;
    head.status(status::_404);
    EXPECT_EQ(head.status(), status::_404);
}

TEST(ResponseHeader, Get_and_set_content_length)
{
    header head;
    EXPECT_EQ(head.contentLength(), 0);
    head.contentLength(450);
    EXPECT_EQ(head.contentLength(), 450);
    head.contentLength(720);
    EXPECT_EQ(head.contentLength(), 720);
}

TEST(ResponseHeader, To_string)
{
    header head;
    EXPECT_EQ(head.toString(), "HTTP/1.1 200 OK\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 0\r\nConnection: Keep-Alive\r\n\r\n");
    head.status(status::_202);
    head.contentLength(450);
    EXPECT_EQ(head.toString(), "HTTP/1.1 202 Accepted\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 450\r\nConnection: Keep-Alive\r\n\r\n");
}

TEST(ResponseHeader, Content_type_can_be_changed)
{
    header head;
    EXPECT_EQ(head.contentType(), header::DEFAULT_CONTENT_TYPE);
    auto imageContentType = "image/jpeg";
    head.contentType(imageContentType);
    EXPECT_EQ(head.contentType(), imageContentType);
}

TEST(ResponseHeader, To_vector)
{
    // Init
    header head;

    // Compute
    auto data = head.toVector();
    auto stringValue = std::string(data.cbegin(), data.cend());

    // Assert
    EXPECT_EQ(stringValue, "HTTP/1.1 200 OK\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 0\r\nConnection: Keep-Alive\r\n\r\n");

    // Compute
    head.status(status::_202);
    head.contentLength(450);
    data = head.toVector();
    stringValue = std::string(data.cbegin(), data.cend());

    // Assert
    EXPECT_EQ(stringValue, "HTTP/1.1 202 Accepted\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 450\r\nConnection: Keep-Alive\r\n\r\n");
}
