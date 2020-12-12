/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once
#include <gmock/gmock.h>
#include "socket_mock.h"
#include "internal/connection/connection.h"
#include "public_folder_mock.h"
#include "router_mock.h"
#include "socket_mock.h"

class BodyEndMatcher
{
public:
    BodyEndMatcher(std::size_t);
    std::size_t size() const;

private:
    std::size_t m_size;
};

class ConnectionFunctionsMock
{
public:
    ConnectionFunctionsMock();

    MOCK_CONST_METHOD3(async_read_header, void(SocketMockWrapper &, std::string &, std::function<void(std::error_code, std::size_t)>));
    MOCK_CONST_METHOD4(async_read_body, void(SocketMockWrapper &, std::string &, BodyEndMatcher, std::function<void(std::error_code, std::size_t)>));
    MOCK_CONST_METHOD3(async_write, void(SocketMockWrapper &, const std::string &, std::function<void(std::error_code, std::size_t)>));
    MOCK_CONST_METHOD1(bodyEndMatcher, BodyEndMatcher(std::size_t));
    MOCK_CONST_METHOD0(maxBodySize, uint32_t());
    MOCK_CONST_METHOD2(sendFile, bool(SocketMockWrapper&, const std::shared_ptr<PublicFileMock> &));
    MOCK_CONST_METHOD2(async_task, void(SocketMockWrapper&, std::function<void()>));

    void createFakePostReadMethods(uint32_t loops);
    void createFakeGetReadMethods(uint32_t loops);
    void createErrorInHeaderRead();
    void createReadIncorrectHeaderData();
    void createErrorInBodyRead();
    void createFakeReadHeader(uint32_t loops, const std::string &dataToRead);
    void createFakeReadBody(uint32_t loops, const std::string &dataToRead);
    void createBodyEndMatcher();
    void createHeaderEndMatcher();
    void createMaxBodySize();
    void createFakeReadPostRequestWithBigBodyLength();
    void createFakeReadFullPostRequestAtHeaderStage();
    void createFakeReadSplittedPostRequestAtHeaderStage();
    void createFakeReadPostThenGet();
    void createFakeAsyncWrite();
    void createFakeWriteError();
    void createFakeAsyncTask();
    void createFakeSendFile(bool returnValue);

private:
    uint32_t m_readExactlyCount, m_readUntilCount;
};
