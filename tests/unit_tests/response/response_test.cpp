#include <gtest/gtest.h>
#include "response/response.h"

using namespace cpphttp::response;

TEST(Response, SetStatus)
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
