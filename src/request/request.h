#pragma once

#include <memory>
#include "header.h"
#include "body.h"

namespace cpphttp
{
    namespace request
    {
        class request
        {
        public:
            request() = default;
            // No Copy
            request(const request &) = delete;
            request &operator=(const request &) = delete;
            // rValue Copy only
            request(request &&toCopy) noexcept;
            request &operator=(request &&toCopy) noexcept;

            bool isReady() const noexcept;
            void setHeader(const std::string &data) noexcept;
            void setBody(const std::string &data) noexcept;
            const header *getHeader() const noexcept;

        private:
            std::unique_ptr<header> m_header;
            std::unique_ptr<body> m_body;
        };
    } // namespace request
} // namespace cpphttp
