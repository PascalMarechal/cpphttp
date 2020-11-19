#include <gtest/gtest.h>
#include <thread>

#include "common/curl.h"
#include "server/server.h"

using namespace cpphttp::server;

TEST(Server, Start_and_stop)
{
    server testServer(9999);
    std::thread testThread([&]() { testServer.start(); });
    auto available = checkUrl("http://localhost:9999");
    EXPECT_TRUE(available);
    testServer.stop();
    testThread.join();
}
