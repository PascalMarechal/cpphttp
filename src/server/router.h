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

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace server
} // namespace cpphttp
