/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "server.h"
#include "internal/asio.h"
#include "internal/connection/connection.h"
#include "internal/connection/match_end_of_header.h"
#include "internal/public_folder/public_folder.h"

#include <optional>
#include <sys/sendfile.h>

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
    inline static void async_write(asio::ip::tcp::socket &socket, const std::string &buffer, std::function<void(std::error_code, std::size_t)> function) noexcept
    {
        asio::async_write(socket, asio::buffer(buffer), asio::transfer_exactly(buffer.size()), function);
    }

    inline static asio::detail::transfer_exactly_t bodyEndMatcher(std::size_t size) noexcept
    {
        return asio::transfer_exactly(size);
    }

    inline uint64_t maxBodySize() const noexcept
    {
        return m_maxBodySize;
    }

    inline static bool sendFile(asio::ip::tcp::socket &socket, const std::unique_ptr<cpphttp::internal::public_file> &file)
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

    inline static void async_task(asio::ip::tcp::socket &socket, std::function<void()> toCall)
    {
        asio::post(socket.get_executor(), toCall);
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

    inline void publicFolder(const std::string &path, const std::string &folderPath)
    {
        m_publicFolder.publicFolder(path, folderPath);
    }

    inline const std::string &publicFolderPath() const noexcept
    {
        return m_publicFolder.publicFolderPath();
    }

    inline const std::string &publicFolderURL() const noexcept
    {
        return m_publicFolder.publicFolderURL();
    }

private:
    void accept() noexcept
    {
        m_socket.emplace(m_context);
        m_acceptor.async_accept(*m_socket, [&](std::error_code error) {
            auto client = std::make_shared<internal::connection<asio::ip::tcp::socket, connectionFunctions, router, internal::public_folder>>(std::move(*m_socket), m_connectionFunctions, m_router, m_publicFolder);
            client->start();
            accept();
        });
    }

    asio::io_context m_context;
    asio::ip::tcp::acceptor m_acceptor;
    std::optional<asio::ip::tcp::socket> m_socket;
    router m_router;
    connectionFunctions m_connectionFunctions;
    internal::public_folder m_publicFolder;
};

server::server(uint32_t port) : m_server_impl(std::make_unique<impl>(port))
{
    signal(SIGPIPE, SIG_IGN);
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

void server::publicFolder(const std::string &path, const std::string &folderPath)
{
    m_server_impl->publicFolder(path, folderPath);
}

const std::string &server::publicFolderPath() const noexcept
{
    return m_server_impl->publicFolderPath();
}

const std::string &server::publicFolderURL() const noexcept
{
    return m_server_impl->publicFolderURL();
}
