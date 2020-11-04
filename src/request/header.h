#pragma once

#include <string>

namespace cpphttp
{
    namespace request
    {
        class header
        {
        public:
            header();
            bool isReady();
            std::string fill(const std::string &data);

        private:
            bool m_isReady;
        };
    } // namespace request
} // namespace cpphttp
