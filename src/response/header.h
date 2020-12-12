/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include <memory>
#include <vector>
#include "request/version.h"
#include "response/status.h"

namespace cpphttp
{
    namespace response
    {
        class header
        {
        public:
            static inline const std::string DEFAULT_CONTENT_TYPE = "text/html; charset=UTF-8";

            header();
            ~header();
            request::version version() const noexcept;
            response::status status() const noexcept;
            uint32_t contentLength() const noexcept;
            const std::string &contentType() const noexcept;

            void status(response::status status) noexcept;
            void contentLength(uint32_t length) noexcept;
            void contentType(const std::string &type) noexcept;
            std::string toString() const noexcept;
            std::vector<uint8_t> toVector() const noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace response
} // namespace cpphttp
