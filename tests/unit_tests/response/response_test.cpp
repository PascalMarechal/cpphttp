#include <gtest/gtest.h>
#include "response/response.h"

using namespace cpphttp::response;

TEST(Response, Set_status)
{
    response res;
    res.status(status::_204);
    EXPECT_EQ(res.toString(), "HTTP/1.1 204 No Content\r\nContent-Length: 0\r\n\r\n");
}

TEST(Response, Write)
{
    response res;
    res.write("<h1>Hello World!</h1>");
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Length: 21\r\n\r\n<h1>Hello World!</h1>");
    res.write("<p>Some Text</p>");
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Length: 37\r\n\r\n<h1>Hello World!</h1><p>Some Text</p>");
}

TEST(Response, End)
{
    response res;
    EXPECT_FALSE(res.hasEnded());
    res.write("Something");
    res.end();
    res.write("Something");
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Length: 9\r\n\r\nSomething");
    EXPECT_TRUE(res.hasEnded());
}

TEST(Response, Send)
{
    response res;
    EXPECT_FALSE(res.hasEnded());
    res.send("Something");
    res.send("Something");
    EXPECT_EQ(res.toString(), "HTTP/1.1 200 OK\r\nContent-Length: 9\r\n\r\nSomething");
}