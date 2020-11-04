#pragma once

namespace cpphttp
{
    namespace request
    {
        enum class method
        {
            GET,
            POST,
            DELETE,
            PUT,
            HEAD,
            PATCH,
            UNKNOWN
        };
    }
} // namespace cpphttp
