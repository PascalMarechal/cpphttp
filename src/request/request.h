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

            void setHeader(const std::string &data) noexcept;
            void setBody(const std::string &data) noexcept;
            void loadParamFromUrl(std::string_view expectedPath) noexcept;

            bool isReady() const noexcept;
            const cpphttp::request::header &header() const;
            bool has(const std::string &name) const noexcept;
            void set(std::string name, std::string value) noexcept;
            const std::string &get(const std::string &name) const noexcept;
            const std::string &getParam(const std::string &name) const noexcept;

            template <typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value, T>>
            void set(std::string name, T value) noexcept
            {
                set(name, std::to_string(value));
            }

            friend bool operator==(const request &lhs, const request &rhs) noexcept;
            friend bool operator!=(const request &lhs, const request &rhs) noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace request
} // namespace cpphttp
