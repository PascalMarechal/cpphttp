#include "params.h"
#include "string.h"
#include <algorithm>

void cpphttp::tools::extractParameters(std::string_view expectedPath, request::request &req)
{
    auto splittedExpectedPath = split(expectedPath, "/");
    auto splittedUrl = split(req.header().getPath(), "/");
    auto pos = 0;
    for (const auto &val : splittedExpectedPath)
    {
        if (splittedUrl.size() == pos)
            return;

        if (val.size() > 1 && val[0] == ':')
            req.set(std::string(val.substr(1)), std::string(splittedUrl[pos]));

        ++pos;
    }
}
