#pragma once

#include "request/request.h"

namespace cpphttp
{
    namespace server
    {
        class router
        {
        public:
            router();
            ~router();
            std::string process(cpphttp::request::request &);

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace server
} // namespace cpphttp
