#pragma once

#include <string>

namespace cpphttp
{
    namespace request
    {
        class body{
            public:
                body(const std::string& data);
                const std::string& getData() noexcept;
                std::size_t getSize() noexcept;
            private:
                std::string m_data;
        };
    }
}
