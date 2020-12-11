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
  // Init
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;

  // Assert
  EXPECT_CALL(functionsMock, async_read_header).Times(0);

  // Compute
  std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(new SocketMock()), functionsMock, routerMock, publicFolderMock);
}

TEST(Connection, Read_request_with_body)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  SocketMockWrapper *wrapper;
  functionsMock.createFakePostReadMethods(2);
  routerMock.createFakeProcess();
  publicFolderMock.createFakeFilePathDoesNotExist();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(3);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(2);
  EXPECT_CALL(functionsMock, bodyEndMatcher(Requests::EXPECTED_POST_BODY_SIZE)).Times(2);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::POST_REQUEST_HEADER, Requests::POST_REQUEST_BODY))).Times(2);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), RouterMock::ExpectedFakeResult, _)).Times(2);
  EXPECT_CALL(functionsMock, maxBodySize).Times(2);
  EXPECT_CALL(*socketMock, close).Times(0);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists).Times(2);
  EXPECT_CALL(publicFolderMock, getFileHeader).Times(0);

  // Compute
  c->start();
}

TEST(Connection, Read_request_without_body)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeGetReadMethods(1);
  publicFolderMock.createFakeFilePathDoesNotExist();
  routerMock.createFakeProcess();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(2);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(0);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::GET_REQUEST_HEADER, ""))).Times(1);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), RouterMock::ExpectedFakeResult, _)).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(*socketMock, close).Times(0);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists).Times(1);
  EXPECT_CALL(publicFolderMock, getFileHeader).Times(0);

  // Compute
  c->start();
}

TEST(Connection, Error_in_header_read)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createErrorInHeaderRead();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  // Compute
  c->start();
}

TEST(Connection, Error_in_body_read)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createErrorInBodyRead();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(1);
  EXPECT_CALL(functionsMock, bodyEndMatcher(Requests::EXPECTED_POST_BODY_SIZE)).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  // Compute
  c->start();
}

TEST(Connection, Incorrect_header_data)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createReadIncorrectHeaderData();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  // Compute
  c->start();
}

TEST(Connection, Should_reject_packet_when_body_size_is_too_big)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeReadPostRequestWithBigBodyLength();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);

  // Compute
  c->start();
}

TEST(Connection, Should_accept_full_packet_at_header_stage)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeReadFullPostRequestAtHeaderStage();
  routerMock.createFakeProcess();
  publicFolderMock.createFakeFilePathDoesNotExist();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(2);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::POST_REQUEST_HEADER, Requests::POST_REQUEST_BODY))).Times(1);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), RouterMock::ExpectedFakeResult, _)).Times(1);
  EXPECT_CALL(*socketMock, close).Times(0);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists).Times(1);
  EXPECT_CALL(publicFolderMock, getFileHeader).Times(0);

  // Compute
  c->start();
}

TEST(Connection, Should_accept_splitted_packet_at_header_stage)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeReadSplittedPostRequestAtHeaderStage();
  routerMock.createFakeProcess();
  publicFolderMock.createFakeFilePathDoesNotExist();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(2);
  EXPECT_CALL(functionsMock, async_read_body).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(functionsMock, bodyEndMatcher).Times(1);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::POST_REQUEST_HEADER, Requests::POST_REQUEST_BODY))).Times(1);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), RouterMock::ExpectedFakeResult, _)).Times(1);
  EXPECT_CALL(*socketMock, close).Times(0);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists).Times(1);
  EXPECT_CALL(publicFolderMock, getFileHeader).Times(0);

  // Compute
  c->start();
}

TEST(Connection, Should_respond_to_two_succecutives_operations)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeReadPostThenGet();
  routerMock.createFakeProcess();
  publicFolderMock.createFakeFilePathDoesNotExist();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(3);
  EXPECT_CALL(functionsMock, async_read_body).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(2);
  EXPECT_CALL(functionsMock, bodyEndMatcher).Times(1);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::POST_REQUEST_HEADER, Requests::POST_REQUEST_BODY))).Times(1);
  EXPECT_CALL(routerMock, process(SameRequest(Requests::GET_REQUEST_HEADER, ""))).Times(1);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), RouterMock::ExpectedFakeResult, _)).Times(2);
  EXPECT_CALL(*socketMock, close).Times(0);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists).Times(2);
  EXPECT_CALL(publicFolderMock, getFileHeader).Times(0);

  // Compute
  c->start();
}

TEST(Connection, Should_handle_write_error_correctly)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeWriteError();
  routerMock.createFakeProcess();
  publicFolderMock.createFakeFilePathDoesNotExist();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body).Times(0);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(functionsMock, bodyEndMatcher).Times(0);
  EXPECT_CALL(routerMock, process).Times(1);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), RouterMock::ExpectedFakeResult, _)).Times(1);
  EXPECT_CALL(*socketMock, close).Times(1);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists).Times(1);
  EXPECT_CALL(publicFolderMock, getFileHeader).Times(0);

  // Compute
  c->start();
}

auto somePath = "data/something.txt";

TEST(Connection, Should_handle_static_file_requests)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeGetReadMethods(2);
  functionsMock.createFakeAsyncTask();
  functionsMock.createFakeSendFile(true);
  publicFolderMock.createFakeFilePathDoesExist(somePath);
  publicFolderMock.createFakeGetFileHeader();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(3);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(0);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), PublicFolderMock::ExpectedFakeResult, _)).Times(2);
  EXPECT_CALL(functionsMock, maxBodySize).Times(2);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(0);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists("/index")).Times(2).WillRepeatedly(Return(somePath));
  EXPECT_CALL(publicFolderMock, getFileHeader(somePath)).Times(2).WillRepeatedly(Return(PublicFolderMock::ExpectedFakeResult));
  EXPECT_CALL(functionsMock, sendFile(matchSocketMock(socketMock), somePath)).Times(2);
  EXPECT_CALL(functionsMock, async_task(matchSocketMock(socketMock), _)).Times(2);

  // Compute
  c->start();
}

TEST(Connection, Should_handle_static_file_send_failure)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeGetReadMethods(2);
  functionsMock.createFakeAsyncTask();
  functionsMock.createFakeSendFile(false);
  publicFolderMock.createFakeFilePathDoesExist(somePath);
  publicFolderMock.createFakeGetFileHeader();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(0);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), PublicFolderMock::ExpectedFakeResult, _)).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists("/index")).Times(1).WillOnce(Return(somePath));
  EXPECT_CALL(publicFolderMock, getFileHeader(somePath)).Times(1).WillOnce(Return(PublicFolderMock::ExpectedFakeResult));
  EXPECT_CALL(functionsMock, sendFile(matchSocketMock(socketMock), somePath)).Times(1);
  EXPECT_CALL(functionsMock, async_task(matchSocketMock(socketMock), _)).Times(1);

  // Compute
  c->start();
}

TEST(Connection, Should_handle_static_file_header_sending_failure)
{
  // Init
  auto socketMock = new SocketMock();
  ConnectionFunctionsMock functionsMock;
  RouterMock routerMock;
  PublicFolderMock publicFolderMock;
  functionsMock.createFakeGetReadMethods(2);
  functionsMock.createFakeWriteError();
  publicFolderMock.createFakeFilePathDoesExist(somePath);
  publicFolderMock.createFakeGetFileHeader();
  auto c = std::make_shared<connection<SocketMockWrapper, ConnectionFunctionsMock, RouterMock, PublicFolderMock>>(SocketMockWrapper(socketMock), functionsMock, routerMock, publicFolderMock);

  // Assert
  EXPECT_CALL(functionsMock, async_read_header(matchSocketMock(socketMock), _, _)).Times(1);
  EXPECT_CALL(functionsMock, async_read_body(matchSocketMock(socketMock), _, matchBodyEndMatcher(), _)).Times(0);
  EXPECT_CALL(functionsMock, async_write(matchSocketMock(socketMock), PublicFolderMock::ExpectedFakeResult, _)).Times(1);
  EXPECT_CALL(functionsMock, maxBodySize).Times(1);
  EXPECT_CALL(routerMock, process).Times(0);
  EXPECT_CALL(*socketMock, close).Times(1);
  EXPECT_CALL(publicFolderMock, getFilePathIfExists("/index")).Times(1).WillOnce(Return(somePath));
  EXPECT_CALL(publicFolderMock, getFileHeader(somePath)).Times(1).WillOnce(Return(PublicFolderMock::ExpectedFakeResult));
  EXPECT_CALL(functionsMock, sendFile).Times(0);
  EXPECT_CALL(functionsMock, async_task).Times(0);

  // Compute
  c->start();
}
