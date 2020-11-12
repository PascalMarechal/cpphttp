#include "request/request.h"
#include <gtest/gtest.h>
#include "common/requests.h"

using namespace cpphttp::request;

TEST(Request, CompleteHeaderWithNoBody)
{
    request req;
    req.setHeader("GET /index HTTP/1.0\n\n");
    EXPECT_TRUE(req.isReady());
}

TEST(Request, CompleteHeaderWithMissingBody)
{
    request req;
    req.setHeader(postRequestHeader);
    EXPECT_FALSE(req.isReady());
}

TEST(Request, CompleteHeaderWithIncompleteBody)
{
    request req;
    req.setHeader(postRequestHeader);
    req.setBody("too short");
    EXPECT_FALSE(req.isReady());
}

TEST(Request, CompleteHeaderWithCompleteBody)
{
    request req;
    req.setHeader(postRequestHeader);
    req.setBody(postRequestBody);
    EXPECT_TRUE(req.isReady());
}

TEST(Request, MoveOperators)
{
    request req;
    req.setHeader(postRequestHeader);
    req.setBody(postRequestBody);

    // Copy constructor
    auto req2 = std::move(req);
    EXPECT_TRUE(req2.isReady());
    EXPECT_FALSE(req.isReady());

    // Asignement
    request req3;
    req3 = std::move(req2);
    EXPECT_TRUE(req3.isReady());
    EXPECT_FALSE(req2.isReady());
}
