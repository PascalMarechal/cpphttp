#include <gtest/gtest.h>
#include "internal/connection.h"

using namespace cpphttp::internal;

TEST(Connection, Creation)
{
    asio::io_context context;
    asio::ip::tcp::socket sock(context);
    connection c(std::move(sock));
}
