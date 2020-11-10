#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "internal/connection.h"

using namespace cpphttp::internal;
class AsyncFunctions
{
public:
  AsyncFunctions()
  {
    CreateFakeMethods();
  }

  static uint32_t expected_write_call_count;
  static uint32_t expected_read_call_count;

  MOCK_CONST_METHOD3(async_write, void(asio::ip::tcp::socket &, asio::mutable_buffer, std::function<void(std::error_code, std::size_t)>));
  MOCK_CONST_METHOD4(async_read_until, void(asio::ip::tcp::socket &, asio::mutable_buffers_1, std::string_view, std::function<void(std::error_code, std::size_t)>));

  void CreateFakeMethods()
  {
    ON_CALL(*this, async_write).WillByDefault([this](asio::ip::tcp::socket &s, asio::mutable_buffer b, std::function<void(std::error_code, std::size_t)> f) {
      std::cout << "Hello There Write\n";
      //f(std::error_code(), b.size());
    });
    ON_CALL(*this, async_read_until).WillByDefault([this](asio::ip::tcp::socket &s, asio::mutable_buffers_1 b, std::string_view delim, std::function<void(std::error_code, std::size_t)> f) {
      std::cout << "Hello There Read\r\n\r\n";
      f(std::error_code(), b.size());
    });

    EXPECT_CALL(*this, async_read_until).Times(expected_read_call_count);
    EXPECT_CALL(*this, async_write).Times(expected_write_call_count);
  }
};

uint32_t AsyncFunctions::expected_write_call_count = 0;
uint32_t AsyncFunctions::expected_read_call_count = 0;

TEST(Connection, Creation)
{
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  connection<AsyncFunctions> c(std::move(sock));
}

TEST(Connection, Read)
{
  AsyncFunctions::expected_read_call_count = 1;
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  connection<AsyncFunctions> c(std::move(sock));
  c.start();
}
