#pragma once

#include <string>

namespace cpphttp
{
    namespace request
    {
        class body{
            public:
                void appendData(const std::string& data);
                std::string& getData() noexcept;
                std::size_t getSize() noexcept;
            private:
                std::string m_data;
        };
    }
}
