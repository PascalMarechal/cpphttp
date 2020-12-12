/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include <string>
#include "request/request.h"
#include "response/response.h"

namespace cpphttp
{
    namespace internal
    {
        class public_file
        {
        public:
            public_file(std::string path);
            const std::string &path() noexcept;
            size_t size() noexcept;
            std::string header() noexcept;

        private:
            std::string m_path;
            size_t m_size;
            inline void initFileSize() noexcept;
            inline void contentType(cpphttp::response::header &head) noexcept;
        };
    } // namespace internal
} // namespace cpphttp
