/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal/connection/connection.h"
#include "common/requests.h"
#include "request/request.h"
#include "common/matchers/request_matcher.h"
#include "mocks/router_mock.h"
#include "mocks/connection_functions_mock.h"

using namespace cpphttp::internal;
using namespace ::testing;

auto matchSocketMock(SocketMock *toMatch)
{
  return AllOf(Property(&SocketMockWrapper::getSocket, toMatch));
}

auto matchBodyEndMatcher()
{
  return AllOf(Property(&BodyEndMatcher::getSize, Requests::EXPECTED_POST_BODY_SIZE));
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

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(3);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(2);
  EXPECT_CALL(functionsMock, bodyEndMatcher(Requests::EXPECTED_POST_BODY_SIZE)).Times(2);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::POST_REQUEST_HEADER, Requests::POST_REQUEST_BODY))).Times(2);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), RouterMock::ExpectedFakeResult, _)).Times(2);
  EXPECT_CALL(functionsMock, maxBodySize).Times(2);
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

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(2);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(0);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::GET_REQUEST_HEADER, ""))).Times(1);
  EXPECT_CALL(functionsMock, async_write).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
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

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
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

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(1);
  EXPECT_CALL(functionsMock, bodyEndMatcher(Requests::EXPECTED_POST_BODY_SIZE)).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
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

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Should_reject_packet_when_body_size_is_too_big)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createFakeReadPostRequestWithBigBodyLength();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Should_accept_full_packet_at_header_stage)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createFakeReadFullPostRequestAtHeaderStage();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(2);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::POST_REQUEST_HEADER, Requests::POST_REQUEST_BODY))).Times(1);
  EXPECT_CALL(functionsMock, async_write).Times(1);
  EXPECT_CALL(*socketMock, close).Times(0);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Should_accept_splitted_packet_at_header_stage)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createFakeReadSplittedPostRequestAtHeaderStage();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(2);
  EXPECT_CALL(functionsMock, async_read_body).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(functionsMock, bodyEndMatcher).Times(1);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::POST_REQUEST_HEADER, Requests::POST_REQUEST_BODY))).Times(1);
  EXPECT_CALL(functionsMock, async_write).Times(1);
  EXPECT_CALL(*socketMock, close).Times(0);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Should_respond_to_two_succecutives_operations)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createFakeReadPostThenGet();
  routerMock.createFakeProcess();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(3);
  EXPECT_CALL(functionsMock, async_read_body).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(2);
  EXPECT_CALL(functionsMock, bodyEndMatcher).Times(1);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::POST_REQUEST_HEADER, Requests::POST_REQUEST_BODY))).Times(1);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::GET_REQUEST_HEADER, ""))).Times(1);
  EXPECT_CALL(functionsMock, async_write).Times(2);
  EXPECT_CALL(*socketMock, close).Times(0);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}

TEST(Connection, Should_handle_write_error_correctly)
{
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;

  functionsMock.createFakeWriteError();
  routerMock.createFakeProcess();

  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(functionsMock, bodyEndMatcher).Times(0);
  EXPECT_CALL(routerMock, process).Times(1);
  EXPECT_CALL(functionsMock, async_write).Times(1);
  EXPECT_CALL(*socketMock, close).Times(1);

  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock);
  c->start();
}
