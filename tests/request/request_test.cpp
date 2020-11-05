#include "request/request.h"
#include <gtest/gtest.h>
#include "../tools/data_reader.h"
#include <unistd.h>

using namespace cpphttp::request;

std::string getRequest = readFile("./data/get_request.txt");
std::string incompletePostRequest = readFile("./data/incomplete_post_request.txt");
extern std::string postRequest;

TEST(Request, EmptyRequest)
{
    request req;
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, CompleteRequest)
{
    request req;
    req.read("GET /index HTTP/1.1\n\n");
    EXPECT_EQ(req.isReady(), true);
}

TEST(Request, IncompleteRequest)
{
    request req;
    req.read("GET /index HTTP/1.1");
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, IncompletePost)
{
    request req;
    req.read(incompletePostRequest);
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, FullPostRequest)
{
    request req;
    req.read(postRequest);
    EXPECT_EQ(req.isReady(), true);
}
