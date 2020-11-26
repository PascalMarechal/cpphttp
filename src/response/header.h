/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include <memory>
#include "request/version.h"
#include "response/status.h"

namespace cpphttp
{
    namespace response
    {
        class header
        {
        public:
            header();
            ~header();
            request::version version() const noexcept;
            response::status status() const noexcept;
            uint32_t contentLength() const noexcept;
            void status(response::status status) noexcept;
            void setContentLength(uint32_t length) noexcept;
            std::string toString() const noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace response
} // namespace cpphttp
