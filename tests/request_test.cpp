#include "request.h"
#include <gtest/gtest.h>
#include "tools/data_reader.h"
#include <unistd.h>

using namespace cpphttp;

std::string getRequest = readFile("./data/get_request.txt");
std::string incomplete = readFile("./data/incomplete_request.txt");

TEST(Request, EmptyRequest)
{
    request req;
    EXPECT_EQ(req.isReady(), false);
}

TEST(Request, CompleteRequest)
{
    request req;
    req.fill(getRequest);
    EXPECT_EQ(req.isReady(), true);
}

TEST(Request, IncompleteRequest)
{
    request req;
    req.fill("GET /index.html HTTP/1.1");
    EXPECT_EQ(req.isReady(), false);
}
