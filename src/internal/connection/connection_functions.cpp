/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 *  
 *  WARNING : This file is not tested directly. It is testing with server.cpp/.h
 */
#include "connection_functions.h"

#include <sys/sendfile.h>

using namespace cpphttp::internal;

connection_functions::connection_functions()
{
    // Header max size is 64 KB by default
    m_matcher.setMaxHeaderSize(64 * 1024);
    // Body max size is 2 MB
    m_maxBodySize = 1024 * 1024 * 2;
}

void connection_functions::async_read_header(asio::ip::tcp::socket &socket,
                                             std::string &buffer,
                                             std::function<void(std::error_code, std::size_t)> function) const noexcept
{
    asio::async_read_until(socket, asio::dynamic_buffer(buffer), m_matcher, function);
}
void connection_functions::async_read_body(asio::ip::tcp::socket &socket,
                                           std::string &buffer,
                                           asio::detail::transfer_exactly_t matcher, std::function<void(std::error_code, std::size_t)> function) noexcept
{
    asio::async_read(socket, asio::dynamic_buffer(buffer), matcher, function);
}
void connection_functions::async_write(asio::ip::tcp::socket &socket, const std::string &buffer, std::function<void(std::error_code, std::size_t)> function) noexcept
{
    asio::async_write(socket, asio::buffer(buffer), asio::transfer_exactly(buffer.size()), function);
}

asio::detail::transfer_exactly_t connection_functions::bodyEndMatcher(std::size_t size) noexcept
{
    return asio::transfer_exactly(size);
}

uint64_t connection_functions::maxBodySize() const noexcept
{
    return m_maxBodySize;
}

bool connection_functions::sendFile(asio::ip::tcp::socket &socket, const std::unique_ptr<cpphttp::internal::public_file> &file)
{
    int flag = 1;
    setsockopt(socket.native_handle(), SOL_SOCKET, MSG_NOSIGNAL, &flag, sizeof(flag));

    auto fd = open(file->path().c_str(), O_RDONLY);
    if (fd < 0)
        return false;

    size_t count = file->size();
    off_t start = 0;
    while (true)
    {
        static uint64_t totalError = 0;
        auto res = sendfile64(socket.native_handle(), fd, &start, count - start);
        if (res == 0)
            return true;
        if (res < 0)
            return false;
    }
}

void connection_functions::async_task(asio::ip::tcp::socket &socket, std::function<void()> toCall)
{
    asio::post(socket.get_executor(), toCall);
}

void connection_functions::maxIncomingHeaderSize(uint64_t size) noexcept
{
    m_matcher.setMaxHeaderSize(size);
}

void connection_functions::maxIncomingBodySize(uint64_t size) noexcept
{
    m_maxBodySize = size;
}
