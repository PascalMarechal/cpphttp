#pragma once

#include "status.h"
#include "header.h"

namespace cpphttp
{
    namespace response
    {
        class response
        {
        public:
            void status(cpphttp::response::status) noexcept;
            std::string toString() noexcept;
            void write(const std::string& data) noexcept;
        private:
            void setHeaderLength() noexcept;

            header m_header;
            std::string m_body;
        };
    } // namespace response
} // namespace cpphttp
