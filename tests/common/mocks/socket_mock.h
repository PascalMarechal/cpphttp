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

private:
    SocketMock *m_socket;
};
