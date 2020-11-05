#pragma once

#include <string>
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
            void read(const std::string &data);

        private:
           header m_header;
           body m_body;
        };
    } // namespace request
} // namespace cpphttp
