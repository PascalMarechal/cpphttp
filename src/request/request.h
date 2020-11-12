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

            bool isReady();
            void setHeader(const std::string &data);
            void setBody(const std::string &data);

        private:
            std::unique_ptr<header> m_header;
            std::unique_ptr<body> m_body;
        };
    } // namespace request
} // namespace cpphttp
