#pragma once
#include <gmock/gmock.h>
#include "socket_mock.h"
#include "internal/asio.h"
#include "internal/connection/match_end_of_header.h"

class ConnectionFunctionsMock
{
public:
    ConnectionFunctionsMock();

    MOCK_CONST_METHOD4(async_read_until, void(SocketMockWrapper &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, cpphttp::internal::match_end_of_header &, std::function<void(std::error_code, std::size_t)>));
    MOCK_CONST_METHOD4(async_read_exactly, void(SocketMockWrapper &, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>>, asio::detail::transfer_exactly_t, std::function<void(std::error_code, std::size_t)>));
    MOCK_CONST_METHOD2(write, void(SocketMockWrapper &, const std::string &));

    void createFakePostReadMethods(uint32_t loops);
    void createFakeGetReadMethods(uint32_t loops);
    void createErrorInHeaderRead();
    void createReadIncorrectHeaderData();
    void createErrorInBodyRead();
    void createFakeReadUntilMethod(uint32_t loops, const std::string &dataToRead);
    void createFakeReadExactlyMethod(uint32_t loops, const std::string &dataToRead);

private:
    uint32_t m_readExactlyCount, m_readUntilCount;
};
