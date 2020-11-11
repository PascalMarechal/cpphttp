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
            bool isReady();
            void setHeader(const std::string& data);
            void setBody(const std::string& data);
        private:

            std::unique_ptr<header> m_header;
            std::unique_ptr<body> m_body;
        };
    } // namespace request
} // namespace cpphttp
