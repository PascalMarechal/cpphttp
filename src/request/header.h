#pragma once

#include <memory>
#include "method.h"
#include "version.h"

#define MAX_HEADER_SIZE 8 * 1024

namespace cpphttp
{
    namespace request
    {
        class header
        {
        public:
            header(const std::string_view &data);
            ~header();

            bool isReady() const noexcept;
            method getMethod() const noexcept;
            version getVersion() const noexcept;
            const std::string &getPath() const noexcept;
            const std::string &getGetParams() const noexcept;
            uint32_t getExpectedBodySize() const noexcept;

            friend bool operator==(const header &lhs, const header &rhs) noexcept;
            friend bool operator!=(const header &lhs, const header &rhs) noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace request
} // namespace cpphttp
