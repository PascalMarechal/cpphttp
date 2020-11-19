#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal/connection/connection.h"
#include "common/requests.h"
#include "request/request.h"
#include "common/matchers/request_matcher.h"
#include "common/mocks/router_mock.h"
#include "common/mocks/connection_functions_mock.h"

using namespace cpphttp::internal;
using namespace ::testing;

auto matchSocketMock(SocketMock *toMatch)
{
  return AllOf(Property(&SocketMockWrapper::getSocket, toMatch));
}

auto matchBodyEndMatcher()
{
  return AllOf(Property(&BodyEndMatcher::getSize, Requests::ExpectedPostBodySize));
}

TEST(Connection, Creation)
{
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  EXPECT_CALL(functionsMock, async_read_header).Times(0);
  std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(new SocketMock()), functionsMock, routerMock);
}

TEST(Connection, Read_request_with_body)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  SocketMockWrapper *wrapper;

  functionsMock.createFakePostReadMethods(2);
  routerMock.createFakeProcess();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _, _)).Times(3);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(2);
  EXPECT_CALL(functionsMock, createBuffer).Times(5);
  EXPECT_CALL(functionsMock, headerEndMatcher).Times(3);
  EXPECT_CALL(functionsMock, bodyEndMatcher(Requests::ExpectedPostBodySize)).Times(2);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::PostRequestHeader, Requests::PostRequestBody))).Times(2);
  EXPECT_CALL(functionsMock, write(matchSocketMock(socketMock), RouterMock::ExpectedFakeResult)).Times(2);
  EXPECT_CALL(*socketMock, close).Times(0);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Read_request_without_body)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createFakeGetReadMethods(1);

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _, _)).Times(2);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(0);
  EXPECT_CALL(functionsMock, createBuffer).Times(2);
  EXPECT_CALL(functionsMock, headerEndMatcher).Times(2);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::GetRequestHeader, ""))).Times(1);
  EXPECT_CALL(functionsMock, write).Times(1);
  EXPECT_CALL(*socketMock, close).Times(0);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Error_in_header_read)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createErrorInHeaderRead();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(functionsMock, createBuffer).Times(1);
  EXPECT_CALL(functionsMock, headerEndMatcher).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Error_in_body_read)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createErrorInBodyRead();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(1);
  EXPECT_CALL(functionsMock, createBuffer).Times(2);
  EXPECT_CALL(functionsMock, headerEndMatcher).Times(1);
  EXPECT_CALL(functionsMock, bodyEndMatcher(Requests::ExpectedPostBodySize)).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Incorrect_header_data)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createReadIncorrectHeaderData();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(functionsMock, createBuffer).Times(1);
  EXPECT_CALL(functionsMock, headerEndMatcher).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}
