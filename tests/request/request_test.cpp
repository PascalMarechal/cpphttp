#include "request/request.h"
#include <gtest/gtest.h>
#include "../tools/data_reader.h"

using namespace cpphttp::request;

std::string postRequestBody = readFile("./data/body/post_request.txt");
extern std::string postRequestHeader;

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
