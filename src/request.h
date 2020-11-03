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
        private:
            bool m_isReady;
    };
} // namespace cpphttp
