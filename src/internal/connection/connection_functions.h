/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 *  
 *  WARNING : This file is not tested directly. It is testing with server.cpp/.h
 */
#pragma once
#include "internal/asio.h"
#include "match_end_of_header.h"
#include "internal/public_folder/public_file.h"

namespace cpphttp
{
    namespace internal
    {
        class connection_functions
        {
        public:
            connection_functions();
            void async_read_header(asio::ip::tcp::socket &socket,
                                   std::string &buffer,
                                   std::function<void(std::error_code, std::size_t)> function) const noexcept;
            static void async_read_body(asio::ip::tcp::socket &socket,
                                        std::string &buffer,
                                        asio::detail::transfer_exactly_t matcher, std::function<void(std::error_code, std::size_t)> function) noexcept;
            static void async_write(asio::ip::tcp::socket &socket, const std::string &buffer, std::function<void(std::error_code, std::size_t)> function) noexcept;

            static asio::detail::transfer_exactly_t bodyEndMatcher(std::size_t size) noexcept;

            uint64_t maxBodySize() const noexcept;
            static bool sendFile(asio::ip::tcp::socket &socket, const std::unique_ptr<cpphttp::internal::public_file> &file);
            static void async_task(asio::ip::tcp::socket &socket, std::function<void()> toCall);
            void setMaxIncomingHeaderSize(uint64_t size) noexcept;
            void setMaxIncomingBodySize(uint64_t size) noexcept;

        private:
            cpphttp::internal::match_end_of_header m_matcher;
            uint64_t m_maxBodySize;
        };
    } // namespace internal
} // namespace cpphttp
