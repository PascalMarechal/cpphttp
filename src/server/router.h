/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include "request/request.h"
#include "response/response.h"
#include <functional>

namespace cpphttp
{
    namespace server
    {
        using error_function = std::function<void(const std::string &, cpphttp::request::request &, cpphttp::response::response &)>;
        using error_callback = std::function<void(const std::string &)>;
        using router_function = std::function<void(cpphttp::request::request &, cpphttp::response::response &, error_callback)>;
        class router
        {
        public:
            inline const static std::string DEFAULT_SERVER_ERROR = "Internal Server Error Detected.";

            router();
            ~router();
            // Only rvalue operators
            router(const router &) = delete;
            router &operator=(const router &) = delete;
            router(router &&toCopy);
            router &operator=(router &&toCopy);

            std::vector<uint8_t> process(cpphttp::request::request &) const;

            void setPublicFolder(const std::string &path, const std::string &folder) noexcept;
            const std::string &getPublicFolder() const noexcept;

            void onError(error_function f) noexcept;
            template <typename T, typename... T2, typename std::enable_if_t<0 != sizeof...(T2), int> = 0>
            void onError(T f, T2... others) noexcept
            {
                static_assert(std::is_convertible_v<T, error_function>, "parameter is not an error function.");
                static_assert(std::is_convertible_v<std::tuple_element_t<0, std::tuple<T2...>>, error_function>, "parameter is not an error function.");
                onError(f);
                onError(others...);
            }

            void onAll(const std::string &pathStartingWith, router_function function) noexcept;
            template <typename T, typename... T2, typename std::enable_if_t<0 != sizeof...(T2), int> = 0>
            void onAll(const std::string &pathStartingWith, T f, T2... others) noexcept
            {
                static_assert(std::is_convertible_v<T, router_function>, "parameter is not a router function.");
                static_assert(std::is_convertible_v<std::tuple_element_t<0, std::tuple<T2...>>, router_function>, "parameter is not a router function.");
                onAll(pathStartingWith, f);
                onAll(pathStartingWith, others...);
            }

            void onGet(const std::string &path, router_function function) noexcept;
            template <typename T, typename... T2, typename std::enable_if_t<0 != sizeof...(T2), int> = 0>
            void onGet(const std::string &path, T f, T2... others) noexcept
            {
                static_assert(std::is_convertible_v<T, router_function>, "parameter is not a router function.");
                static_assert(std::is_convertible_v<std::tuple_element_t<0, std::tuple<T2...>>, router_function>, "parameter is not a router function.");
                onGet(path, f);
                onGet(path, others...);
            }

            void onPost(const std::string &path, router_function function) noexcept;
            template <typename T, typename... T2, typename std::enable_if_t<0 != sizeof...(T2), int> = 0>
            void onPost(const std::string &path, T f, T2... others) noexcept
            {
                static_assert(std::is_convertible_v<T, router_function>, "parameter is not a router function.");
                static_assert(std::is_convertible_v<std::tuple_element_t<0, std::tuple<T2...>>, router_function>, "parameter is not a router function.");
                onPost(path, f);
                onPost(path, others...);
            }

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace server
} // namespace cpphttp
