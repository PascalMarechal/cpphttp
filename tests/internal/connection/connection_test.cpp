#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "internal/connection/connection.h"
#include "common/requests.h"
#include "request/request.h"

using namespace cpphttp::internal;

class RouterMock
{
public:
  MOCK_CONST_METHOD1(process, std::string(const cpphttp::request::request &));
};

class ConnectionFunctionsMock
{
public:
  MOCK_CONST_METHOD4(async_read_until, void(asio::ip::tcp::socket &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, match_end_of_header &, std::function<void(std::error_code, std::size_t)>));
  MOCK_CONST_METHOD4(async_read_exactly, void(asio::ip::tcp::socket &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, asio::detail::transfer_exactly_t, std::function<void(std::error_code, std::size_t)>));
  MOCK_CONST_METHOD2(async_work, void(asio::any_io_executor, std::function<void(void)>));

  void createFakeReadMethods()
  {
    createFakeReadUntilMethod();
    createFakeReadExactlyMethod();
  }

  void createFakeReadUntilMethod()
  {
    ON_CALL(*this, async_read_until).WillByDefault([this](asio::ip::tcp::socket &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, match_end_of_header &matcher, std::function<void(std::error_code, std::size_t)> callback) {
      buffer.grow(postRequestHeader.size());
      memcpy(buffer.data(0, postRequestHeader.size()).data(), postRequestHeader.c_str(), postRequestHeader.size());
      // Call handler
      callback(std::error_code(), buffer.size());
    });
  }

  void createFakeReadExactlyMethod()
  {
    ON_CALL(*this, async_read_exactly).WillByDefault([this](asio::ip::tcp::socket &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, asio::detail::transfer_exactly_t end, std::function<void(std::error_code, std::size_t)> callback) {
      buffer.grow(postRequestBody.size());
      memcpy(buffer.data(0, postRequestBody.size()).data(), postRequestBody.c_str(), postRequestBody.size());
      // Call handler
      callback(std::error_code(), buffer.size());
    });
  }

  void createFakeAsyncMethod()
  {
    ON_CALL(*this, async_work).WillByDefault([this](asio::any_io_executor e, std::function<void(void)> callback) {
      callback();
    });
  }
};

TEST(Connection, Creation)
{
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  EXPECT_CALL(functionsMock, async_read_until).Times(0);
  std::make_shared<connection<ConnectionFunctionsMock, RouterMock>>(std::move(sock), functionsMock, routerMock);
}

TEST(Connection, ReadRequestWithBody)
{
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  ConnectionFunctionsMock functionsMock;
  functionsMock.createFakeReadMethods();
  functionsMock.createFakeAsyncMethod();
  RouterMock routerMock;
  EXPECT_CALL(functionsMock, async_read_until).Times(1);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(1);
  EXPECT_CALL(functionsMock, async_work).Times(1);
  EXPECT_CALL(routerMock, process).Times(1);
  auto c = std::make_shared<connection<ConnectionFunctionsMock, RouterMock>>(std::move(sock), functionsMock, routerMock);
  c->start();
}
