#pragma once

#include "request/request.h"
#include "response/response.h"
#include <functional>

namespace cpphttp
{
    namespace server
    {
        using error_function = std::function<void(const std::string &, cpphttp::request::request &, cpphttp::response::response &)>;
        class router
        {
        public:
            router();
            ~router();
            std::string process(cpphttp::request::request &);

            void error(error_function f);

            template <typename T, typename... T2, typename std::enable_if<0 != sizeof...(T2), int>::type = 0>
            void error(T f, T2... others)
            {
                static_assert(std::is_convertible_v<T, error_function>, "Is not a function");
                error(f);
                error(others...);
            }

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace server
} // namespace cpphttp
