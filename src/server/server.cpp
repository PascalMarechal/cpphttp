#include "server.h"
#include "internal/asio.h"
#include <optional>

using namespace cpphttp::server;

class server::impl
{
public:
    impl(uint32_t port) : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
        accept();
    }

    void start()
    {
        m_context.run();
    }

    void stop()
    {
        m_context.stop();
    }

private:
    void accept()
    {
        m_socket.emplace(m_context);

        m_acceptor.async_accept(*m_socket, [&](std::error_code error) {
            asio::write(*m_socket, asio::buffer("HTTP/1.1 200 OK\r\nContent-Length: 9\r\n\r\nSomething"));
            accept();
        });
    }

    asio::io_context m_context;
    asio::ip::tcp::acceptor m_acceptor;
    std::optional<asio::ip::tcp::socket> m_socket;
};

server::server(uint32_t port) : m_server_impl(std::make_unique<impl>(port))
{
}

server::~server()
{
}

void server::start()
{
    m_server_impl->start();
}

void server::stop()
{
    m_server_impl->stop();
}