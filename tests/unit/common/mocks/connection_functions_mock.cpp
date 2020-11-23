#include "connection_functions_mock.h"
#include "common/requests.h"

BodyEndMatcher::BodyEndMatcher(std::size_t s) : m_size(s)
{
}

std::size_t BodyEndMatcher::getSize() const
{
    return m_size;
}

ConnectionFunctionsMock::ConnectionFunctionsMock() : m_readExactlyCount(0), m_readUntilCount(0)
{
    createMaxBodySize();
}

void ConnectionFunctionsMock::createFakePostReadMethods(uint32_t loops)
{
    createBufferFunction();
    createFakeReadHeader(loops, Requests::PostRequestHeader);
    createFakeReadBody(loops, Requests::PostRequestBody);
}

void ConnectionFunctionsMock::createFakeGetReadMethods(uint32_t loops)
{
    createBufferFunction();
    createFakeReadHeader(loops, Requests::GetRequestHeader);
    createFakeReadBody(loops, "");
}

void ConnectionFunctionsMock::createFakeReadPostRequestWithBigBodyLength()
{
    createBufferFunction();
    createFakeReadHeader(1, Requests::PostRequestHeaderWithBigBodyLength);
}

void ConnectionFunctionsMock::createErrorInHeaderRead()
{
    createBufferFunction();
    ON_CALL(*this, async_read_header).WillByDefault([this](SocketMockWrapper &socket, std::string &buffer, HeaderEndMatcher matcher, std::function<void(std::error_code, std::size_t)> callback) {
        callback(std::make_error_code(std::errc::io_error), 0);
    });
}

void ConnectionFunctionsMock::createReadIncorrectHeaderData()
{
    createBufferFunction();
    createFakeReadHeader(1, "wrongHeaderData\r\n\r\n");
}

void ConnectionFunctionsMock::createErrorInBodyRead()
{
    createBufferFunction();
    createBodyEndMatcher();
    createFakeReadHeader(1, Requests::PostRequestHeader);
    ON_CALL(*this, async_read_body).WillByDefault([this](SocketMockWrapper &socket, std::string &buffer, BodyEndMatcher matcher, std::function<void(std::error_code, std::size_t)> callback) {
        callback(std::make_error_code(std::errc::io_error), 0);
    });
}

void ConnectionFunctionsMock::createFakeReadHeader(uint32_t loops, const std::string &dataToRead)
{
    createHeaderEndMatcher();
    ON_CALL(*this, async_read_header).WillByDefault([this, loops, dataToRead](SocketMockWrapper &socket, std::string &buffer, HeaderEndMatcher matcher, std::function<void(std::error_code, std::size_t)> callback) {
        if (++m_readUntilCount > loops)
            return;

        buffer = dataToRead;
        callback(std::error_code(), buffer.size());
    });
}

void ConnectionFunctionsMock::createFakeReadBody(uint32_t loops, const std::string &dataToRead)
{
    createBodyEndMatcher();
    ON_CALL(*this, async_read_body).WillByDefault([this, loops, dataToRead](SocketMockWrapper &socket, std::string &buffer, BodyEndMatcher matcher, std::function<void(std::error_code, std::size_t)> callback) {
        if (++m_readExactlyCount > loops)
            return;
        buffer = dataToRead;
        callback(std::error_code(), buffer.size());
    });
}

void ConnectionFunctionsMock::createBufferFunction()
{
    ON_CALL(*this, createBuffer).WillByDefault([](std::string &buffer) -> std::string & {
        return buffer;
    });
}

void ConnectionFunctionsMock::createBodyEndMatcher()
{
    ON_CALL(*this, bodyEndMatcher).WillByDefault([](std::size_t size) -> BodyEndMatcher {
        return BodyEndMatcher(size);
    });
}

void ConnectionFunctionsMock::createHeaderEndMatcher()
{
    ON_CALL(*this, headerEndMatcher).WillByDefault([]() -> HeaderEndMatcher {
        return HeaderEndMatcher();
    });
}

void ConnectionFunctionsMock::createMaxBodySize()
{
    ON_CALL(*this, maxBodySize).WillByDefault(testing::Return(100));
}