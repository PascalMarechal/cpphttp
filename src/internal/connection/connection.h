/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once
#include <functional>
#include <memory>
#include "request/request.h"

namespace cpphttp
{
    namespace internal
    {
        template <typename Socket, typename ConnectionFunctions, typename Router, typename PublicFolder>
        class connection : public std::enable_shared_from_this<connection<Socket, ConnectionFunctions, Router, PublicFolder>>
        {
        public:
            connection(Socket &&sock, const ConnectionFunctions &functions, const Router &router, const PublicFolder &publicFolder)
                : m_socket(std::move(sock)), m_functions(functions), m_router(router), m_publicFolder(publicFolder){};

            void start()
            {
                reset();
                readHeader();
            }

        private:
            inline void readHeader() noexcept
            {
                m_functions.async_read_header(m_socket, m_buffer, std::bind(&connection::onReadHeader, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            inline void readBody(std::size_t bytesToRead) noexcept
            {
                m_functions.async_read_body(m_socket, m_buffer, m_functions.bodyEndMatcher(bytesToRead), std::bind(&connection::onReadBody, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }

            void onReadHeader(std::error_code error, std::size_t bytesTransferred) noexcept
            {
                if (error || !headerSetup(bytesTransferred))
                    return exit(error);

                auto bytesToRead = bodyBytesToRead(bytesTransferred);
                if (bytesToRead)
                    return readBody(bytesToRead);

                bodySetup(bytesTransferred);
                processAndReadNextRequest();
            }

            void onReadBody(std::error_code error, std::size_t bytesTransferred) noexcept
            {
                if (error)
                    return exit(error);
                bodySetup(m_headerSize);
                processAndReadNextRequest();
            }

            void processAndReadNextRequest() noexcept
            {
                m_file = m_publicFolder.publicFile(m_currentRequest->header().getPath());
                if (m_file)
                    m_functions.async_write(m_socket, m_file->header(), std::bind(&connection::onWriteStaticFileHeader, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
                else
                {
                    auto response = m_router.process(*m_currentRequest);
                    m_functions.async_write(m_socket, response, std::bind(&connection::onWrite, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
                }
            }

            void onWriteStaticFileHeader(std::error_code error, std::size_t bytesTransferred) noexcept
            {
                if (error)
                    return exit(error);

                m_functions.async_task(m_socket, [ptr = this->shared_from_this()]() {
                    if (ptr->m_functions.sendFile(ptr->m_socket, ptr->m_file))
                        ptr->start();
                    else
                        ptr->exit(std::make_error_code(std::errc::io_error));
                });
            }

            void onWrite(std::error_code error, std::size_t bytesTransferred) noexcept
            {
                if (error)
                    return exit(error);
                start();
            }

            inline void exit(std::error_code error) noexcept
            {
                m_socket.close(error);
            }

            inline bool headerSetup(std::size_t bytesTransferred) noexcept
            {
                std::string_view bufferView = m_buffer;
                m_headerSize = bytesTransferred;
                m_currentRequest->setHeader(bufferView.substr(0, m_headerSize));
                return m_currentRequest->header().isReady() && m_currentRequest->header().getExpectedBodySize() <= m_functions.maxBodySize();
            }

            inline void bodySetup(std::size_t from) noexcept
            {
                auto size = m_currentRequest->header().getExpectedBodySize();
                if (!size)
                    return;
                std::string_view bufferView = m_buffer;
                m_currentRequest->setBody(bufferView.substr(from, size));
            }

            inline std::size_t bodyBytesToRead(std::size_t bytesTransferred) noexcept
            {
                auto expectedBodySize = m_currentRequest->header().getExpectedBodySize();
                if (!expectedBodySize)
                    return 0;

                auto extra = m_buffer.size() - bytesTransferred;
                if (extra < expectedBodySize)
                    return expectedBodySize - extra;

                return 0;
            }

            inline void reset() noexcept
            {
                m_buffer.clear();
                m_currentRequest = std::make_unique<request::request>();
            }

            Socket m_socket;
            const ConnectionFunctions &m_functions;
            const Router &m_router;
            const PublicFolder &m_publicFolder;

            std::string m_buffer;
            std::size_t m_headerSize;
            std::unique_ptr<request::request> m_currentRequest;
            decltype(m_publicFolder.publicFile("")) m_file;
        };
    } // namespace internal
} // namespace cpphttp
