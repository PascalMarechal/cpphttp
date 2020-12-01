/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "server.h"
#include "internal/asio.h"
#include <optional>
#include "internal/connection/connection.h"
#include "internal/connection/match_end_of_header.h"

using namespace cpphttp::server;

class connectionFunctions
{
public:
    inline void async_read_header(asio::ip::tcp::socket &socket,
                                         asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer,
                                         const cpphttp::internal::match_end_of_header &matcher,
                                         std::function<void(std::error_code, std::size_t)> function) const noexcept
    {
        asio::async_read_until(socket, buffer, matcher, function);
    }
    inline static void async_read_body(asio::ip::tcp::socket &socket,
                                       asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> buffer,
                                       asio::detail::transfer_exactly_t matcher, std::function<void(std::error_code, std::size_t)> function) noexcept
    {
        asio::async_read(socket, buffer, matcher, function);
    }
    inline static void write(asio::ip::tcp::socket &socket, const std::string &buffer) noexcept
    {
        asio::write(socket, asio::buffer(buffer));
    }

    inline static asio::dynamic_string_buffer<char, std::char_traits<char>, std::allocator<char>> createBuffer(std::string &from) noexcept
    {
        return asio::dynamic_buffer(from);
    }

    inline const cpphttp::internal::match_end_of_header &headerEndMatcher() const noexcept
    {
        return m_matcher;
    }

    inline static asio::detail::transfer_exactly_t bodyEndMatcher(std::size_t size) noexcept
    {
        return asio::transfer_exactly(size);
    }

    inline static uint32_t maxBodySize() noexcept
    {
        return 1024 * 1024 * 2; // 2M
    }

    inline void setMaxHeaderSize(uint64_t size) noexcept
    {
        m_matcher.setMaxHeaderSize(size);
    }

private:
    cpphttp::internal::match_end_of_header m_matcher;
};

class server::impl
{
public:
    impl(uint32_t port) : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
        accept();
    }

    inline void start() noexcept
    {
        m_context.run();
    }

    inline void stop() noexcept
    {
        m_context.stop();
    }

    inline void setRouter(router &&router) noexcept
    {
        m_router = std::move(router);
    }

    inline void setMaxHeaderSize(u_int64_t size) noexcept
    {
        m_connectionFunctions.setMaxHeaderSize(size);
    }

private:
    void accept() noexcept
    {
        m_socket.emplace(m_context);
        m_acceptor.async_accept(*m_socket, [&](std::error_code error) {
            auto client = std::make_shared<internal::connection<asio::ip::tcp::socket, connectionFunctions, router>>(std::move(*m_socket), m_connectionFunctions, m_router);
            client->start();
            accept();
        });
    }

    asio::io_context m_context;
    asio::ip::tcp::acceptor m_acceptor;
    std::optional<asio::ip::tcp::socket> m_socket;
    router m_router;
    connectionFunctions m_connectionFunctions;
};

server::server(uint32_t port) : m_server_impl(std::make_unique<impl>(port))
{
}

server::~server() noexcept
{
}

void server::start() noexcept
{
    m_server_impl->start();
}

void server::stop() noexcept
{
    m_server_impl->stop();
}

void server::setRouter(router &&router) noexcept
{
    m_server_impl->setRouter(std::move(router));
}

void server::setMaxHeaderSize(u_int64_t size) noexcept
{
    m_server_impl->setMaxHeaderSize(size);
}
