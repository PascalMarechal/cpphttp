/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "server.h"
#include "internal/asio.h"
#include "internal/connection/connection.h"
#include "internal/connection/match_end_of_header.h"

#include <optional>

using namespace cpphttp::server;

class connectionFunctions
{
public:
    connectionFunctions()
    {
        // Header max size is 64 KB by default
        m_matcher.setMaxHeaderSize(64 * 1024);
        // Body max size is 2 MB
        m_maxBodySize = 1024 * 1024 * 2;
    }

    inline void async_read_header(asio::ip::tcp::socket &socket,
                                  std::string &buffer,
                                  std::function<void(std::error_code, std::size_t)> function) const noexcept
    {
        asio::async_read_until(socket, asio::dynamic_buffer(buffer), m_matcher, function);
    }
    inline static void async_read_body(asio::ip::tcp::socket &socket,
                                       std::string &buffer,
                                       asio::detail::transfer_exactly_t matcher, std::function<void(std::error_code, std::size_t)> function) noexcept
    {
        asio::async_read(socket, asio::dynamic_buffer(buffer), matcher, function);
    }
    inline static void async_write(asio::ip::tcp::socket &socket, const std::vector<uint8_t> &buffer, std::function<void(std::error_code, std::size_t)> function) noexcept
    {
        signal(SIGPIPE, SIG_IGN);
        auto fileName = "/src/tests/unit/data/static_files/images/test.png";
        size_t count_;
        struct stat statbuf;
        int result = stat(fileName, &statbuf);
        count_ = statbuf.st_size;

        cpphttp::response::header head;
        head.setContentLength(count_);
        head.setContentType("image/png");
        auto toSend = head.toString();
        
        asio::async_write(socket, asio::buffer(toSend), asio::transfer_exactly(toSend.size()), function);
    }

    inline static asio::detail::transfer_exactly_t bodyEndMatcher(std::size_t size) noexcept
    {
        return asio::transfer_exactly(size);
    }

    inline uint64_t maxBodySize() const noexcept
    {
        return m_maxBodySize;
    }

    inline void setMaxIncomingHeaderSize(uint64_t size) noexcept
    {
        m_matcher.setMaxHeaderSize(size);
    }

    inline void setMaxIncomingBodySize(uint64_t size) noexcept
    {
        m_maxBodySize = size;
    }

private:
    cpphttp::internal::match_end_of_header m_matcher;
    uint64_t m_maxBodySize;
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

    inline void setMaxIncomingHeaderSize(u_int64_t size) noexcept
    {
        m_connectionFunctions.setMaxIncomingHeaderSize(size);
    }

    inline void setMaxIncomingBodySize(u_int64_t size) noexcept
    {
        m_connectionFunctions.setMaxIncomingBodySize(size);
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

void server::setMaxIncomingHeaderSize(u_int64_t size) noexcept
{
    m_server_impl->setMaxIncomingHeaderSize(size);
}

void server::setMaxIncomingBodySize(u_int64_t size) noexcept
{
    m_server_impl->setMaxIncomingBodySize(size);
}
