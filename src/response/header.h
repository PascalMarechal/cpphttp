#pragma once

#include <string>
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
            request::version version() const noexcept;
            response::status status() const noexcept;
            uint32_t contentLength() const noexcept;
            void setStatus(response::status status) noexcept;
            void setContentLength(uint32_t length) noexcept;
            std::string toString() const noexcept;

        private:
            response::status m_status;
            uint32_t m_contentLength;
        };
    } // namespace response
} // namespace cpphttp
