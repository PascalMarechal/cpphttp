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
            std::string process(cpphttp::request::request &) const;

            void error(error_function f) noexcept;

            template <typename T, typename... T2, typename std::enable_if_t<0 != sizeof...(T2), int> = 0>
            void error(T f, T2... others) noexcept
            {
                static_assert(std::is_convertible_v<T, error_function>, "parameter is not an error function.");
                static_assert(std::is_convertible_v<std::tuple_element_t<0, std::tuple<T2...>>, error_function>, "parameter is not an error function.");
                error(f);
                error(others...);
            }

            void use(const std::string &pathStartingWith, router_function function) noexcept;

            template <typename T, typename... T2, typename std::enable_if_t<0 != sizeof...(T2), int> = 0>
            void use(const std::string &pathStartingWith, T f, T2... others) noexcept
            {
                static_assert(std::is_convertible_v<T, router_function>, "parameter is not a router function.");
                static_assert(std::is_convertible_v<std::tuple_element_t<0, std::tuple<T2...>>, router_function>, "parameter is not a router function.");
                use(pathStartingWith, f);
                use(pathStartingWith, others...);
            }

            void get(const std::string &path, router_function function) noexcept;
            template <typename T, typename... T2, typename std::enable_if_t<0 != sizeof...(T2), int> = 0>
            void get(const std::string &path, T f, T2... others) noexcept
            {
                static_assert(std::is_convertible_v<T, router_function>, "parameter is not a router function.");
                static_assert(std::is_convertible_v<std::tuple_element_t<0, std::tuple<T2...>>, router_function>, "parameter is not a router function.");
                get(path, f);
                get(path, others...);
            }

            void post(const std::string &path, router_function function) noexcept;
            template <typename T, typename... T2, typename std::enable_if_t<0 != sizeof...(T2), int> = 0>
            void post(const std::string &path, T f, T2... others) noexcept
            {
                static_assert(std::is_convertible_v<T, router_function>, "parameter is not a router function.");
                static_assert(std::is_convertible_v<std::tuple_element_t<0, std::tuple<T2...>>, router_function>, "parameter is not a router function.");
                post(path, f);
                post(path, others...);
            }

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace server
} // namespace cpphttp
