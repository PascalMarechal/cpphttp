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
    head.setContentLength(450);
    EXPECT_EQ(head.contentLength(), 450);
    head.setContentLength(720);
    EXPECT_EQ(head.contentLength(), 720);
}

TEST(ResponseHeader, To_string)
{
    header head;
    EXPECT_EQ(head.toString(), "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    head.status(status::_202);
    head.setContentLength(450);
    EXPECT_EQ(head.toString(), "HTTP/1.1 202 Accepted\r\nContent-Length: 450\r\n\r\n");
}
