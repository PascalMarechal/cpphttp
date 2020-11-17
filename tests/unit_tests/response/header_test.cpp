#include <gtest/gtest.h>
#include "response/header.h"

using namespace cpphttp;
using namespace response;

TEST(ResponseHeader, ShouldBeV1_1ByDefault)
{
    header head;
    EXPECT_EQ(head.version(), request::version::_1_1);
}

TEST(ResponseHeader, StatusShouldBe200AtStart)
{
    header head;
    EXPECT_EQ(head.status(), status::_200);
}

TEST(ResponseHeader, StatusCanBeChanged)
{
    header head;
    head.status(status::_404);
    EXPECT_EQ(head.status(), status::_404);
}

TEST(ResponseHeader, GetAndSetContentLength)
{
    header head;
    EXPECT_EQ(head.contentLength(), 0);
    head.setContentLength(450);
    EXPECT_EQ(head.contentLength(), 450);
    head.setContentLength(720);
    EXPECT_EQ(head.contentLength(), 720);
}

TEST(ResponseHeader, ToString)
{
    header head;
    EXPECT_EQ(head.toString(), "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    head.status(status::_202);
    head.setContentLength(450);
    EXPECT_EQ(head.toString(), "HTTP/1.1 202 Accepted\r\nContent-Length: 450\r\n\r\n");
}
