#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "internal/connection/connection.h"
#include "common/requests.h"

using namespace cpphttp::internal;

class ConnectionFunctionsMock
{
public:
  MOCK_CONST_METHOD4(async_read_until, void(asio::ip::tcp::socket &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, match_end_of_header &, std::function<void(std::error_code, std::size_t)>));

  void createFakeReadMethod(uint32_t expectedReadCalls)
  {
    ON_CALL(*this, async_read_until).WillByDefault([this](asio::ip::tcp::socket &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, match_end_of_header &matcher, std::function<void(std::error_code, std::size_t)> callback) {
      // Copy header
      buffer.grow(postRequestHeader.size());
      memcpy(buffer.data(0, postRequestHeader.size()).data(), postRequestHeader.c_str(), postRequestHeader.size());
      // Call handler
      callback(std::error_code(), buffer.size());
    });
  }
};

TEST(Connection, Creation)
{
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  ConnectionFunctionsMock mock;
  EXPECT_CALL(mock, async_read_until).Times(0);
  auto c = std::make_shared<connection<ConnectionFunctionsMock>>(std::move(sock), mock);
}

TEST(Connection, Read)
{
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  ConnectionFunctionsMock mock;
  EXPECT_CALL(mock, async_read_until).Times(1);
  auto c = std::make_shared<connection<ConnectionFunctionsMock>>(std::move(sock), mock);
  c->start();
}
