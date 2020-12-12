/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal/public_folder/public_file.h"

using namespace cpphttp::internal;
using ::testing::HasSubstr;

auto wrongPath = "/any/path";

TEST(PublicFile, Get_file_path)
{
    // Init
    public_file publicFile(wrongPath);

    // Assert
    EXPECT_EQ(publicFile.path(), wrongPath);
}

TEST(PublicFile, Missing_file_should_return_zero_size)
{
    // Init
    public_file publicFile(wrongPath);

    // Assert
    EXPECT_EQ(publicFile.size(), 0);
}

TEST(PublicFile, Missing_file_should_return_empty_header)
{
    // Init
    public_file publicFile(wrongPath);

    // Assert
    EXPECT_TRUE(publicFile.header().empty());
}

auto correctPath = "data/read_test.txt";

TEST(PublicFile, Should_return_correct_file_size)
{
    // Init
    public_file publicFile(correctPath);

    // Assert
    EXPECT_EQ(publicFile.size(), 20);
}

TEST(PublicFile, Should_return_correct_header)
{
    // Init
    public_file publicFile(correctPath);

    // Assert
    EXPECT_THAT(publicFile.header(), HasSubstr("Content-Length: 20"));
}
