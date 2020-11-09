#include "connection.h"

using namespace cpphttp::internal;

connection::connection(asio::ip::tcp::socket &&socket) : m_socket(std::move(socket))
{
}
