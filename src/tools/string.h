#pragma once

#include <vector>
#include <string>

namespace cpphttp
{
    namespace tools
    {
        std::vector<std::string_view> split(std::string_view strv, std::string_view delims = " ");
    }
} // namespace cpphttp
