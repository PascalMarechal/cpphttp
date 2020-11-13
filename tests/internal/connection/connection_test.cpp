#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "internal/connection/connection.h"
#include "common/requests.h"
#include "request/request.h"
#include "common/request_matcher.h"

using namespace cpphttp::internal;

class RouterMock
{
public:
  MOCK_CONST_METHOD1(process, std::string(const cpphttp::request::request &));
};

class ConnectionFunctionsMock
{
private:
  uint32_t m_readExactlyCount, m_readUntilCount;

public:
  ConnectionFunctionsMock() : m_readExactlyCount(0), m_readUntilCount(0) {}

  MOCK_CONST_METHOD4(async_read_until, void(asio::ip::tcp::socket &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, match_end_of_header &, std::function<void(std::error_code, std::size_t)>));
  MOCK_CONST_METHOD4(async_read_exactly, void(asio::ip::tcp::socket &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, asio::detail::transfer_exactly_t, std::function<void(std::error_code, std::size_t)>));

  void createFakePostReadMethods(uint32_t loops)
  {
    createFakeReadUntilMethod(loops, postRequestHeader);
    createFakeReadExactlyMethod(loops, postRequestBody);
  }

  void createFakeGetReadMethods(uint32_t loops)
  {
    createFakeReadUntilMethod(loops, getRequestHeader);
    createFakeReadExactlyMethod(loops, "");
  }

  void createFakeReadUntilMethod(uint32_t loops, const std::string &dataToRead)
  {
    ON_CALL(*this, async_read_until).WillByDefault([this, loops, dataToRead](asio::ip::tcp::socket &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, match_end_of_header &matcher, std::function<void(std::error_code, std::size_t)> callback) {
      if (++m_readUntilCount > loops)
        return;

      buffer.grow(dataToRead.size());
      memcpy(buffer.data(0, dataToRead.size()).data(), dataToRead.c_str(), dataToRead.size());
      // Call handler
      callback(std::error_code(), buffer.size());
    });
  }

  void createFakeReadExactlyMethod(uint32_t loops, const std::string &dataToRead)
  {
    ON_CALL(*this, async_read_exactly).WillByDefault([this, loops, dataToRead](asio::ip::tcp::socket &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, asio::detail::transfer_exactly_t end, std::function<void(std::error_code, std::size_t)> callback) {
      if (++m_readExactlyCount > loops)
        return;
      buffer.grow(dataToRead.size());
      memcpy(buffer.data(0, dataToRead.size()).data(), dataToRead.c_str(), dataToRead.size());
      // Call handler
      callback(std::error_code(), buffer.size());
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
  functionsMock.createFakePostReadMethods(2);
  RouterMock routerMock;
  EXPECT_CALL(functionsMock, async_read_until).Times(3);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(2);
  EXPECT_CALL(routerMock, process(SameRequest(postRequestHeader, postRequestBody))).Times(2);
  auto c = std::make_shared<connection<ConnectionFunctionsMock, RouterMock>>(std::move(sock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, ReadRequestWithoutBody)
{
  asio::io_context context;
  asio::ip::tcp::socket sock(context);
  ConnectionFunctionsMock functionsMock;
  functionsMock.createFakeGetReadMethods(1);
  RouterMock routerMock;
  EXPECT_CALL(functionsMock, async_read_until).Times(2);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(0);
  EXPECT_CALL(routerMock, process(SameRequest(getRequestHeader, ""))).Times(1);
  auto c = std::make_shared<connection<ConnectionFunctionsMock, RouterMock>>(std::move(sock), functionsMock, routerMock);
  c->start();
}
