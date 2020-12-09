/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include <vector>
#include <string>

namespace cpphttp
{
    namespace internal
    {
        std::vector<std::string_view> split(const std::string_view &strv, const std::string_view &delims = " ");
        std::string uriDecode(const std::string_view &toDecode);
        std::string readFile(const std::string& path);
        std::vector<uint8_t> readBinaryFile(const std::string& path);
    } // namespace internal
} // namespace cpphttp
