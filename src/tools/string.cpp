#include "string.h"
#include <algorithm>

std::vector<std::string_view>
cpphttp::tools::split(std::string_view strv, std::string_view delims)
{
    std::vector<std::string_view> output;
    auto first = strv.begin();

    while (first != strv.end())
    {
        const auto second = std::find_first_of(first, std::cend(strv),
                                               std::cbegin(delims), std::cend(delims));
        if (first != second)
        {
            output.emplace_back(strv.substr(std::distance(strv.begin(), first), std::distance(first, second)));
        }

        if (second == strv.end())
            break;

        first = std::next(second);
    }

    return output;
}
