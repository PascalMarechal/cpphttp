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
            std::string read(const std::string &data);

        private:

            inline std::string appendToBody(const std::string& data) noexcept;

            header m_header;
            body m_body;
        };
    } // namespace request
} // namespace cpphttp
