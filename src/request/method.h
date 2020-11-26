/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
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
