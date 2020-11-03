#pragma once

#include <string>

namespace cpphttp
{
    class request
    {
        public:
            request();
            bool isReady();
            void fill(const std::string& data);
            void setBodySize(uint32_t size);
        private:

            void handleHeaderLine(const std::string& line);

            bool m_isReady;
            uint32_t m_bodySize;
            std::string m_body;
    };
} // namespace cpphttp
