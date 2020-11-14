#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal/connection/connection.h"
#include "common/requests.h"
#include "request/request.h"
#include "common/matchers/request_matcher.h"
#include "common/mocks/router_mock.h"
#include "common/mocks/connection_functions_mock.h"

using namespace cpphttp::internal;

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
  SocketMockWrapper* wrapper;

  functionsMock.createFakePostReadMethods(2);
  routerMock.createFakeProcess();

  EXPECT_CALL(functionsMock, async_read_until).Times(3);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(2);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::PostRequestHeader, Requests::PostRequestBody))).Times(2);
  EXPECT_CALL(functionsMock, write(testing::AllOf(testing::Property(&SocketMockWrapper::getSocket, socketMock)), RouterMock::ExpectedFakeResult)).Times(2);
  EXPECT_CALL(*socketMock, close).Times(0);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, ReadRequestWithoutBody)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createFakeGetReadMethods(1);

  EXPECT_CALL(functionsMock, async_read_until).Times(2);
  EXPECT_CALL(functionsMock, async_read_exactly).Times(0);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::GetRequestHeader, ""))).Times(1);
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
