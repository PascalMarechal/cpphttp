#pragma once

#include <vector>
#include <string>

namespace cpphttp
{
    namespace internal
    {
        std::vector<std::string_view> split(const std::string_view &strv, const std::string_view &delims = " ");
        std::string uriDecode(const std::string_view &toDecode);
    } // namespace internal
} // namespace cpphttp
