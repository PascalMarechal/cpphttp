#pragma once

#include <string>

namespace cpphttp
{
    namespace request
    {
        class request
        {
        public:
            request();
            bool isReady();
            void fill(const std::string &data);
            void setExpectedBodySize(uint32_t size);

        private:
            void handleHeaderLine(const std::string &line);

            bool m_isReady;
            uint32_t m_expectedBodySize;
            std::string m_body;
        };
    } // namespace request
} // namespace cpphttp
