/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
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

TEST(Server, Has_default_empty_router)
{
    server testServer(9999);
    std::thread testThread([&]() { testServer.start(); });
    auto page = getPage("http://localhost:9999");
    EXPECT_EQ(page, router::DEFAULT_SERVER_ERROR);
    testServer.stop();
    testThread.join();
}

TEST(Server, Can_use_defined_router_functions)
{
    server testServer(9999);
    router r;
    std::string message = "<h1>Hello World!</h1>";
    r.onGet("/url", [&](cpphttp::request::request &req, cpphttp::response::response &res, error_callback error) {
        res.send(message);
    });
    testServer.setRouter(std::move(r));
    std::thread testThread([&]() { testServer.start(); });

    auto page = getPage("http://localhost:9999/url");
    EXPECT_EQ(page, message);

    testServer.stop();
    testThread.join();
}

TEST(Server, Can_set_max_header_size)
{
    server testServer(9999);
    router r;
    std::string message = "<h1>Hello World!</h1>";
    r.onGet("/url", [&](cpphttp::request::request &req, cpphttp::response::response &res, error_callback error) {
        res.send(message);
    });
    testServer.setRouter(std::move(r));
    testServer.setMaxIncomingHeaderSize(6);
    std::thread testThread([&]() { testServer.start(); });

    auto page = getPage("http://localhost:9999/url");
    EXPECT_EQ(page, "");

    testServer.stop();
    testThread.join();
}

TEST(Server, Can_set_max_body_size)
{
    server testServer(9999);
    router r;
    std::string message = "<h1>Hello World!</h1>";
    r.onPost("/post", [&](cpphttp::request::request &req, cpphttp::response::response &res, error_callback error) {
        res.send(message);
    });
    testServer.setRouter(std::move(r));
    testServer.setMaxIncomingBodySize(12);
    std::thread testThread([&]() { testServer.start(); });

    auto page = postPage("http://localhost:9999/post", "name=daniel&project=curl");
    EXPECT_EQ(page, "");
    page = postPage("http://localhost:9999/post", "name=daniel");
    EXPECT_EQ(page, message);

    testServer.stop();
    testThread.join();
}
