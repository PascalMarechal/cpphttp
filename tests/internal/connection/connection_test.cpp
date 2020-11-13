#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "internal/connection/connection.h"
#include "common/requests.h"
#include "request/request.h"
#include "common/request_matcher.h"

using namespace cpphttp::internal;

std::string expectedRouterResult = "<h1>Router Result</h1>";

class RouterMock
{
public:
  MOCK_CONST_METHOD1(process, std::string(const cpphttp::request::request &));

  void createFakeProcess()
  {
    ON_CALL(*this, process).WillByDefault(testing::Return(expectedRouterResult));
  }
};

class SocketMock
{
public:
  MOCK_METHOD1(close, void(std::error_code));
};

class SocketMockWrapper
{
public:
  SocketMockWrapper(SocketMock *socket) : m_socket(socket){};
  SocketMockWrapper(SocketMockWrapper &&from)
  {
    m_socket = from.m_socket;
    from.m_socket = nullptr;
  }

  ~SocketMockWrapper()
  {
    if (m_socket != nullptr)
      delete m_socket;
  }
  void close(std::error_code e)
  {
    m_socket->close(e);
  }

private:
  SocketMock *m_socket;
};

class ConnectionFunctionsMock
{
private:
  uint32_t m_readExactlyCount, m_readUntilCount;

public:
  ConnectionFunctionsMock() : m_readExactlyCount(0), m_readUntilCount(0) {}

  MOCK_CONST_METHOD4(async_read_until, void(SocketMockWrapper &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, match_end_of_header &, std::function<void(std::error_code, std::size_t)>));
  MOCK_CONST_METHOD4(async_read_exactly, void(SocketMockWrapper &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, asio::detail::transfer_exactly_t, std::function<void(std::error_code, std::size_t)>));
  MOCK_CONST_METHOD2(write, void(SocketMockWrapper &, const std::string &));

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

  void createErrorInHeaderRead()
  {
    ON_CALL(*this, async_read_until).WillByDefault([this](SocketMockWrapper &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, match_end_of_header &matcher, std::function<void(std::error_code, std::size_t)> callback) {
      callback(std::make_error_code(std::errc::io_error), 0);
    });
  }

  void createReadIncorrectHeaderData()
  {
    createFakeReadUntilMethod(1, "wrongHeaderData\r\n\r\n");
  }

  void createErrorInBodyRead()
  {
    createFakeReadUntilMethod(1, postRequestHeader);
    ON_CALL(*this, async_read_exactly).WillByDefault([this](SocketMockWrapper &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, asio::detail::transfer_exactly_t end, std::function<void(std::error_code, std::size_t)> callback) {
      callback(std::make_error_code(std::errc::io_error), 0);
    });
  }

  void createFakeReadUntilMethod(uint32_t loops, const std::string &dataToRead)
  {
    ON_CALL(*this, async_read_until).WillByDefault([this, loops, dataToRead](SocketMockWrapper &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, match_end_of_header &matcher, std::function<void(std::error_code, std::size_t)> callback) {
      if (++m_readUntilCount > loops)
        return;

      buffer.grow(dataToRead.size());
      memcpy(buffer.data(0, dataToRead.size()).data(), dataToRead.c_str(), dataToRead.size());
      callback(std::error_code(), buffer.size());
    });
  }

  void createFakeReadExactlyMethod(uint32_t loops, const std::string &dataToRead)
  {
    ON_CALL(*this, async_read_exactly).WillByDefault([this, loops, dataToRead](SocketMockWrapper &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, asio::detail::transfer_exactly_t end, std::function<void(std::error_code, std::size_t)> callback) {
      if (++m_readExactlyCount > loops)
        return;
      buffer.grow(dataToRead.size());
      memcpy(buffer.data(0, dataToRead.size()).data(), dataToRead.c_str(), dataToRead.size());
      callback(std::error_code(), buffer.size());
    });
  }
};

TEST(Connection, Creation)
{
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  EXPECT_CALL(functionsMock, async_read_until).Times(0);
  std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(new SocketMock()), functionsMock, routerMock);
}

TEST(Connection, ReadRequestWithBody)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  std::string writtenValue;

  functionsMock.createFakePostReadMethods(2);
  routerMock.createFakeProcess();

  EXPECT_CALL(functionsMock, async_read_until).Times(3);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(2);
  EXPECT_CALL(routerMock, process(SameRequest(postRequestHeader, postRequestBody))).Times(2);
  EXPECT_CALL(functionsMock, write).Times(2).WillOnce(testing::SaveArg<1>(&writtenValue));
  EXPECT_CALL(*socketMock, close).Times(0);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();

  EXPECT_EQ(writtenValue, expectedRouterResult);
}

TEST(Connection, ReadRequestWithoutBody)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createFakeGetReadMethods(1);

  EXPECT_CALL(functionsMock, async_read_until).Times(2);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(0);
  EXPECT_CALL(routerMock, process(SameRequest(getRequestHeader, ""))).Times(1);
  EXPECT_CALL(functionsMock, write).Times(1);
  EXPECT_CALL(*socketMock, close).Times(0);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, ErrorInHeaderRead)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createErrorInHeaderRead();

  EXPECT_CALL(functionsMock, async_read_until).Times(1);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(0);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, ErrorInBodyRead)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createErrorInBodyRead();

  EXPECT_CALL(functionsMock, async_read_until).Times(1);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, IncorrectHeaderData)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createReadIncorrectHeaderData();

  EXPECT_CALL(functionsMock, async_read_until).Times(1);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(0);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}
