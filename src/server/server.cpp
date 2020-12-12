/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "server.h"
#include "internal/connection/connection_functions.h"
#include "internal/connection/connection.h"
#include "internal/public_folder/public_folder.h"

#include <optional>

using namespace cpphttp::server;
using namespace cpphttp::internal;

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

    inline void router(cpphttp::server::router &&router) noexcept
    {
        m_router = std::move(router);
    }

    inline void maxIncomingHeaderSize(u_int64_t size) noexcept
    {
        m_connectionFunctions.maxIncomingHeaderSize(size);
    }

    inline void maxIncomingBodySize(u_int64_t size) noexcept
    {
        m_connectionFunctions.maxIncomingBodySize(size);
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
            auto client = std::make_shared<connection<asio::ip::tcp::socket, connection_functions, cpphttp::server::router, public_folder>>(std::move(*m_socket), m_connectionFunctions, m_router, m_publicFolder);
            client->start();
            accept();
        });
    }

    asio::io_context m_context;
    asio::ip::tcp::acceptor m_acceptor;
    std::optional<asio::ip::tcp::socket> m_socket;
    cpphttp::server::router m_router;
    connection_functions m_connectionFunctions;
    public_folder m_publicFolder;
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

void server::router(cpphttp::server::router &&router) noexcept
{
    m_server_impl->router(std::move(router));
}

void server::maxIncomingHeaderSize(u_int64_t size) noexcept
{
    m_server_impl->maxIncomingHeaderSize(size);
}

void server::maxIncomingBodySize(u_int64_t size) noexcept
{
    m_server_impl->maxIncomingBodySize(size);
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
