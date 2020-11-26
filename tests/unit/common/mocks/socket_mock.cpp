/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "socket_mock.h"

SocketMockWrapper::SocketMockWrapper(SocketMock *socket) : m_socket(socket) {}

SocketMockWrapper::SocketMockWrapper(SocketMockWrapper &&from)
{
    m_socket = from.m_socket;
    from.m_socket = nullptr;
}

SocketMockWrapper::~SocketMockWrapper()
{
    if (m_socket != nullptr)
        delete m_socket;
}
void SocketMockWrapper::close(std::error_code e)
{
    m_socket->close(e);
}

SocketMock *SocketMockWrapper::getSocket() const
{
    return m_socket;
}
