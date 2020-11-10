#include <gtest/gtest.h>
#include "internal/connection/match_end_of_header.h"

using namespace cpphttp::internal;

TEST(MATCHER, FailToFindEnd)
{
    std::string no_end_in_string = "eazeazeaz\n";
    match_end_of_header matcher;
    auto result = matcher(no_end_in_string.cbegin(), no_end_in_string.cend());
    EXPECT_FALSE(result.second);
}
