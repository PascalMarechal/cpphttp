#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "internal/connection/connection.h"

using namespace cpphttp::internal;

extern std::string postRequestHeader;

class AsyncFunctions
{
public:
  static uint32_t expected_read_call_count;

  AsyncFunctions()
  {
    CreateFakeMethods();
  }

  MOCK_CONST_METHOD4(async_read_until, void(asio::ip::tcp::socket &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, match_end_of_header &, std::function<void(std::error_code, std::size_t)>));

  void CreateFakeMethods()
  {
    ON_CALL(*this, async_read_until).WillByDefault([this](asio::ip::tcp::socket &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, match_end_of_header &matcher, std::function<void(std::error_code, std::size_t)> callback) {
      // Copy header
      buffer.grow(postRequestHeader.size());
      memcpy(buffer.data(0, postRequestHeader.size()).data(), postRequestHeader.c_str(), postRequestHeader.size());
      // Call handler
      callback(std::error_code(), buffer.size());
    });

    EXPECT_CALL(*this, async_read_until).Times(expected_read_call_count);
  }
};

uint32_t AsyncFunctions::expected_read_call_count = 0;

TEST(Connection, Creation)
{
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  auto c = std::make_shared<connection<AsyncFunctions>>(std::move(sock));
}

TEST(Connection, Read)
{
  AsyncFunctions::expected_read_call_count = 1;
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  auto c = std::make_shared<connection<AsyncFunctions>>(std::move(sock));
  c->start();
}
