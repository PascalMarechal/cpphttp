/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once
#include <gmock/gmock.h>

class SocketMock
{
public:
    MOCK_METHOD1(close, void(std::error_code));
};

class SocketMockWrapper
{
public:
    SocketMockWrapper(SocketMock *socket);
    SocketMockWrapper(SocketMockWrapper &&from);
    ~SocketMockWrapper();

    void close(std::error_code e);
    SocketMock* getSocket() const;

private:
    SocketMock *m_socket;
};
