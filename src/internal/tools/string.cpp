#include "string.h"
#include "extended_ascii_table.h"
#include <algorithm>
#include <unordered_map>

std::vector<std::string_view>
cpphttp::internal::split(const std::string_view &strv, const std::string_view &delims)
{
    std::vector<std::string_view> output;
    auto first = strv.begin();

    while (first != strv.end())
    {
        const auto second = std::find_first_of(first, std::cend(strv),
                                               std::cbegin(delims), std::cend(delims));
        if (first != second)
            output.emplace_back(strv.substr(std::distance(strv.begin(), first), std::distance(first, second)));

        if (second == strv.end())
            break;

        first = std::next(second);
    }

    return output;
}

const std::unordered_map<char, int> hexTable{
    {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'a', 10}, {'A', 10}, {'b', 11}, {'B', 11}, {'c', 12}, {'C', 12}, {'d', 13}, {'D', 13}, {'e', 14}, {'E', 14}, {'f', 15}, {'F', 15}, {'x', 0}, {'X', 0}};

int toDecimal(const std::string_view &toDecode)
{
    auto firstChar = hexTable.find(toDecode[0]);
    if (firstChar == hexTable.cend())
        return -1;

    auto secondChar = hexTable.find(toDecode[1]);
    if (secondChar == hexTable.cend())
        return -1;

    return (firstChar->second << 4) + secondChar->second;
}

std::string cpphttp::internal::uriDecode(const std::string_view &toDecode)
{
    if (!toDecode.size())
        return "";

    std::string result;
    auto start = toDecode.cbegin();
    for (int i = 0; i < (int)toDecode.size() - 2; ++i)
    {
        if (toDecode[i] != '%')
            continue;

        auto dec = toDecimal(toDecode.substr(i + 1, 2));
        if (dec <= 0)
            continue;

        result.append(start, toDecode.cbegin() + i);
        if (dec <= 127)
            result += (char)dec;
        else
            result += EXTENDED_ASCII_TABLE[dec - 128];
        i += 2;
        start = toDecode.cbegin() + i + 1;
    }

    result.append(start, toDecode.cend());

    return result;
}
