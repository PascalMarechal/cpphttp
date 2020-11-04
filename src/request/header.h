#pragma once

#include <string>

namespace cpphttp
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
} // namespace cpphttp
