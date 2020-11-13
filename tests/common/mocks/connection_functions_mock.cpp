#include "connection_functions_mock.h"
#include "common/requests.h"

ConnectionFunctionsMock::ConnectionFunctionsMock() : m_readExactlyCount(0), m_readUntilCount(0)
{
}

void ConnectionFunctionsMock::createFakePostReadMethods(uint32_t loops)
{
    createFakeReadUntilMethod(loops, Requests::PostRequestHeader);
    createFakeReadExactlyMethod(loops, Requests::PostRequestBody);
}

void ConnectionFunctionsMock::createFakeGetReadMethods(uint32_t loops)
{
    createFakeReadUntilMethod(loops, Requests::GetRequestHeader);
    createFakeReadExactlyMethod(loops, "");
}

void ConnectionFunctionsMock::createErrorInHeaderRead()
{
    ON_CALL(*this, async_read_until).WillByDefault([this](SocketMockWrapper &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, cpphttp::internal::match_end_of_header &matcher, std::function<void(std::error_code, std::size_t)> callback) {
        callback(std::make_error_code(std::errc::io_error), 0);
    });
}

void ConnectionFunctionsMock::createReadIncorrectHeaderData()
{
    createFakeReadUntilMethod(1, "wrongHeaderData\r\n\r\n");
}

void ConnectionFunctionsMock::createErrorInBodyRead()
{
    createFakeReadUntilMethod(1, Requests::PostRequestHeader);
    ON_CALL(*this, async_read_exactly).WillByDefault([this](SocketMockWrapper &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, asio::detail::transfer_exactly_t end, std::function<void(std::error_code, std::size_t)> callback) {
        callback(std::make_error_code(std::errc::io_error), 0);
    });
}

void ConnectionFunctionsMock::createFakeReadUntilMethod(uint32_t loops, const std::string &dataToRead)
{
    ON_CALL(*this, async_read_until).WillByDefault([this, loops, dataToRead](SocketMockWrapper &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, cpphttp::internal::match_end_of_header &matcher, std::function<void(std::error_code, std::size_t)> callback) {
        if (++m_readUntilCount > loops)
            return;

        buffer.grow(dataToRead.size());
        memcpy(buffer.data(0, dataToRead.size()).data(), dataToRead.c_str(), dataToRead.size());
        callback(std::error_code(), buffer.size());
    });
}

void ConnectionFunctionsMock::createFakeReadExactlyMethod(uint32_t loops, const std::string &dataToRead)
{
    ON_CALL(*this, async_read_exactly).WillByDefault([this, loops, dataToRead](SocketMockWrapper &socket, asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer, asio::detail::transfer_exactly_t end, std::function<void(std::error_code, std::size_t)> callback) {
        if (++m_readExactlyCount > loops)
            return;
        buffer.grow(dataToRead.size());
        memcpy(buffer.data(0, dataToRead.size()).data(), dataToRead.c_str(), dataToRead.size());
        callback(std::error_code(), buffer.size());
    });
}
