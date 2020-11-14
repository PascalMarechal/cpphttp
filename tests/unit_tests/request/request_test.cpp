#include "request/request.h"
#include <gtest/gtest.h>
#include "common/requests.h"

using namespace cpphttp::request;

TEST(Request, CompleteHeaderWithNoBody)
{
    request req;
    req.setHeader(Requests::GetRequestHeader);
    EXPECT_TRUE(req.isReady());
}

TEST(Request, CompleteHeaderWithMissingBody)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    EXPECT_FALSE(req.isReady());
}

TEST(Request, CompleteHeaderWithIncompleteBody)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    req.setBody("too short");
    EXPECT_FALSE(req.isReady());
}

TEST(Request, CompleteHeaderWithCompleteBody)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    req.setBody(Requests::PostRequestBody);
    EXPECT_TRUE(req.isReady());
}

TEST(Request, MoveOperators)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    req.setBody(Requests::PostRequestBody);

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

TEST(Request, GetHeader)
{
    request req;
    EXPECT_THROW(req.header(), std::invalid_argument);

    req.setHeader("Something");
    EXPECT_NO_THROW(req.header());
}

TEST(Request, EqualityOperators)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    req.setBody(Requests::PostRequestBody);
    request req2;
    req2.setHeader(Requests::PostRequestHeader);
    req2.setBody(Requests::PostRequestBody);
    EXPECT_TRUE(req == req2);

    request req3, req4;
    EXPECT_TRUE(req3 == req4);
    EXPECT_TRUE(req != req3);

    req3.setHeader(Requests::PostRequestHeader);
    EXPECT_TRUE(req3 != req);

    req3.setBody("nooope");
    EXPECT_TRUE(req3 != req);

    req4.setBody(Requests::PostRequestBody);
    EXPECT_TRUE(req4 != req);

    req2.setHeader("noope");
    EXPECT_TRUE(req != req2);
}