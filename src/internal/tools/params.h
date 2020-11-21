#pragma once

#include "request/request.h"

namespace cpphttp
{
    namespace tools
    {
        void extractParameters(std::string_view expectedPath, request::request &req);
    }
} // namespace cpphttp
