/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include "internal/connection/match_end_of_header.h"

using namespace cpphttp::internal;

TEST(Matcher, Fail_to_find_end)
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

TEST(Matcher, Find_end_of_header_with_CRLF)
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

TEST(Matcher, Find_end_of_header_with_LF_only)
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

TEST(Matcher, Set_max_header_size)
{
    std::string correctHeader = "MyHeader\n\n";
    match_end_of_header matcher;
    matcher.setMaxHeaderSize(10000);
    auto result = matcher(correctHeader.cbegin(), correctHeader.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(correctHeader.cend() == result.first);

    matcher.setMaxHeaderSize(5);
    result = matcher(correctHeader.cbegin(), correctHeader.cend());
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(correctHeader.cbegin() == result.first);
}
