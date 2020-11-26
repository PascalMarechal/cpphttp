/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include <random>
#include "internal/tools/string.h"

using namespace cpphttp::internal;

void checkSplitWith(std::initializer_list<const char *> data, const std::string &separator = " ")
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
    checkSplitWith({"Hello", "World", "There"});
    // Multiple separators
    checkSplitWith({"Hello", "World", "There", "Hello", "World", "There", "Hello", "World", "There", "Hello", "World", "There"}, ",|");
}

TEST(Strings, URI_decode)
{
    auto decodedValue = uriDecode("hello%40test.fr");
    EXPECT_EQ(decodedValue, "hello@test.fr");
    decodedValue = uriDecode("");
    EXPECT_EQ(decodedValue, "");
    decodedValue = uriDecode("100% should not be modified");
    EXPECT_EQ(decodedValue, "100% should not be modified");
    decodedValue = uriDecode("%keep percents%");
    EXPECT_EQ(decodedValue, "%keep percents%");
    decodedValue = uriDecode("%C3%B9");
    EXPECT_EQ(decodedValue, "ù");
    decodedValue = uriDecode("%C3%BF");
    EXPECT_EQ(decodedValue, "ÿ");
    decodedValue = uriDecode("%E2%82%AC");
    EXPECT_EQ(decodedValue, "€");
    decodedValue = uriDecode("%C3%A9%C3%A9%C3%A7%C3%A7");
    EXPECT_EQ(decodedValue, "ééçç");
    decodedValue = uriDecode("%00%C3%A9%C3%A9%C3%A7%C3%A7");
    EXPECT_EQ(decodedValue, "%00ééçç");
}
