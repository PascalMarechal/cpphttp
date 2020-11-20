#pragma once

#include "request/request.h"

namespace cpphttp
{
    namespace tools
    {
        void extractParameters(std::string_view expectedPath, request::request &req);
        void extractTailingParameters(std::string_view urlTail, request::request &req);
    }
} // namespace cpphttp
