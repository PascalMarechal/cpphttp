#include "request/request.h"
#include <gtest/gtest.h>
#include "tools/data_reader.h"
#include <unistd.h>

using namespace cpphttp::request;

std::string getRequest = readFile("./data/get_request.txt");
std::string incompletePostRequest = readFile("./data/incomplete_post_request.txt");

TEST(Request, EmptyRequest)
{
    request req;
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, CompleteRequest)
{
    request req;
    req.fill("GET /index HTTP/1.1\n\n");
    EXPECT_EQ(req.isReady(), true);
}

TEST(Request, IncompleteRequest)
{
    request req;
    req.fill("GET /index HTTP/1.1");
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, IncompletePost)
{
    request req;
    req.fill(incompletePostRequest);
    EXPECT_EQ(req.isReady(), false);
}
