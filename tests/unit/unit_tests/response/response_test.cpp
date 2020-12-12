/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include "response/response.h"

using namespace cpphttp::response;

std::string endOfHeader = "\r\nConnection: Keep-Alive\r\n\r\n";

TEST(Response, Set_status)
{
    response res;
    res.status(status::_204);
    EXPECT_EQ(res.toString(), "HTTP/1.1 204 No Content\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 0" + endOfHeader);
}

TEST(Response, Write)
{
    response res;
    res.write("<h1>Hello World!</h1>");
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 21" + endOfHeader + "<h1>Hello World!</h1>");
    res.write("<p>Some Text</p>");
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 37" + endOfHeader + "<h1>Hello World!</h1><p>Some Text</p>");
}

TEST(Response, End)
{
    response res;
    EXPECT_FALSE(res.hasEnded());
    res.write("Something");
    res.end();
    res.write("Something");
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 9" + endOfHeader + "Something");
    EXPECT_TRUE(res.hasEnded());
}

TEST(Response, Send)
{
    response res;
    EXPECT_FALSE(res.hasEnded());
    res.send("Something");
    res.send("Something");
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 9" + endOfHeader + "Something");
}

TEST(Response, Can_access_to_header_directly)
{
    response res;
    EXPECT_EQ(res.header().contentType(), header::DEFAULT_CONTENT_TYPE);
}

TEST(Response, To_vector)
{
    // Init
    response res;
    res.status(status::_204);

    // Compute
    auto data = res.toVector();
    auto stringValue = std::string(data.cbegin(), data.cend());

    // Assert
    EXPECT_EQ(stringValue, "HTTP/1.1 204 No Content\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 0" + endOfHeader);

    // Compute
    res.write("<h1>Hello World!</h1>");
    res.status(status::_200);
    data = res.toVector();
    stringValue = std::string(data.cbegin(), data.cend());

    // Assert
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Type:" + cpphttp::response::header::DEFAULT_CONTENT_TYPE + "\r\nContent-Length: 21" + endOfHeader + "<h1>Hello World!</h1>");
}

TEST(Response, Send_binary_data)
{
    // Init
    response res;
    std::vector<uint8_t> toSend({48, 0, 49, 0});

    // Compute
    res.send(std::move(toSend));
    auto result = res.toVector();

    // Assert
    EXPECT_TRUE(result.size() > 4);
    EXPECT_EQ(*(result.cend() - 4), 48);
    EXPECT_EQ(*(result.cend() - 3), 0);
    EXPECT_EQ(*(result.cend() - 2), 49);
    EXPECT_EQ(*(result.cend() - 1), 0);
}
