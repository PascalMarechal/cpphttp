/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include "internal/connection/match_end_of_header.h"

using namespace cpphttp::internal;

TEST(Matcher, FailToFindEnd)
{
    std::string no_end_in_string = "something\n";
    match_end_of_header matcher;
    auto result = matcher(no_end_in_string.cbegin(), no_end_in_string.cend());
    EXPECT_FALSE(result.second);
    no_end_in_string = "something\r\n";
    result = matcher(no_end_in_string.cbegin(), no_end_in_string.cend());
    EXPECT_FALSE(result.second);
    no_end_in_string = "something\r\n\r";
    result = matcher(no_end_in_string.cbegin(), no_end_in_string.cend());
    EXPECT_FALSE(result.second);
    no_end_in_string = "something\r\n\r\r";
    result = matcher(no_end_in_string.cbegin(), no_end_in_string.cend());
    EXPECT_FALSE(result.second);
    no_end_in_string = "something\r\n\r\r\n";
    result = matcher(no_end_in_string.cbegin(), no_end_in_string.cend());
    EXPECT_FALSE(result.second);
    no_end_in_string = "something\r\n\r\r\n\r";
    result = matcher(no_end_in_string.cbegin(), no_end_in_string.cend());
    EXPECT_FALSE(result.second);
}

TEST(Matcher, FindEndOfHeaderWithCRLF)
{
    std::string CRLF_end_in_string = "something\r\n\r\n";
    match_end_of_header matcher;
    auto result = matcher(CRLF_end_in_string.cbegin(), CRLF_end_in_string.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(CRLF_end_in_string.cend() == result.first);

    CRLF_end_in_string = "something\r\n\r\r\n\r\n";
    result = matcher(CRLF_end_in_string.cbegin(), CRLF_end_in_string.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(CRLF_end_in_string.cend() == result.first);

    CRLF_end_in_string = "\r\n\r\nsomething";
    result = matcher(CRLF_end_in_string.cbegin(), CRLF_end_in_string.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(CRLF_end_in_string.cbegin() + 4 == result.first);
}

TEST(Matcher, FindEndOfHeaderWithLFOnly)
{
    std::string LF_end_in_string = "something\r\n\n";
    match_end_of_header matcher;
    auto result = matcher(LF_end_in_string.cbegin(), LF_end_in_string.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(LF_end_in_string.cend() == result.first);

    LF_end_in_string = "something\n\n";
    result = matcher(LF_end_in_string.cbegin(), LF_end_in_string.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(LF_end_in_string.cend() == result.first);

    LF_end_in_string = "something\nsomething\nsomething\n\n";
    result = matcher(LF_end_in_string.cbegin(), LF_end_in_string.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(LF_end_in_string.cend() == result.first);

    LF_end_in_string = "\n\nsomething";
    result = matcher(LF_end_in_string.cbegin(), LF_end_in_string.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(LF_end_in_string.cbegin() + 2 == result.first);
}
