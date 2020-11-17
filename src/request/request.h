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
            request();
            ~request();
            // No Copy
            request(const request &) = delete;
            request &operator=(const request &) = delete;
            request(request &&toCopy) = delete;
            request &operator=(request &&toCopy) = delete;

            bool isReady() const noexcept;
            void setHeader(const std::string &data) noexcept;
            void setBody(const std::string &data) noexcept;
            const cpphttp::request::header& header() const;

            friend bool operator==(const request &lhs, const request &rhs) noexcept;
            friend bool operator!=(const request &lhs, const request &rhs) noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace request
} // namespace cpphttp
