#include <gtest/gtest.h>
#include <random>
#include "internal/tools/string.h"

using namespace cpphttp::tools;

void checkSplitValueWith(std::initializer_list<const char *> data, const std::string &separator = " ")
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, separator.size() - 1);

    std::string toSplit;
    for (auto &part : data)
    {
        toSplit += part;
        toSplit += separator.at(dist(rng));
    }
    auto splittedValues = split(toSplit, separator);
    EXPECT_EQ(splittedValues.size(), data.size());
    if (splittedValues.size() == data.size())
    {
        auto i = 0;
        for (auto &part : data)
        {
            EXPECT_EQ(part, splittedValues[i++]);
        }
    }
}

TEST(Strings, Splitting_function)
{
    // One separator
    checkSplitValueWith({"Hello", "World", "There"});
    // Multiple separators
    checkSplitValueWith({"Hello", "World", "There", "Hello", "World", "There", "Hello", "World", "There", "Hello", "World", "There"}, ",|");
}
