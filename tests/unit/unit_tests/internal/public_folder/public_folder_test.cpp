/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal/public_folder/public_folder.h"
#include "common/requests.h"

using namespace cpphttp::internal;
using ::testing::HasSubstr;

TEST(PublicFolder, Setting_public_folder_will_always_set_absolute_path_and_URL)
{
    // Init
    public_folder publicFolder;
    std::filesystem::path cwd = std::filesystem::current_path();

    // Compute
    publicFolder.publicFolder("public", "./relative");
    auto folder = publicFolder.publicFolderPath();
    auto URL = publicFolder.publicFolderURL();

    // Assert
    EXPECT_EQ(folder, cwd.string() + "/relative");
    EXPECT_EQ(URL, "/public/");

    // Compute
    publicFolder.publicFolder("data", "/absolute/path");
    folder = publicFolder.publicFolderPath();
    URL = publicFolder.publicFolderURL();

    // Assert
    EXPECT_EQ(URL, "/data/");
    EXPECT_EQ(folder, "/absolute/path");
}

TEST(PublicFolder, Setting_empty_public_folder_should_throw)
{
    // Init
    public_folder publicFolder;

    // Assert
    EXPECT_THROW(publicFolder.publicFolder("", "./relative"), std::invalid_argument);
    EXPECT_THROW(publicFolder.publicFolder("somewhere", ""), std::invalid_argument);
}

TEST(PublicFolder, Get_file_if_public_folder_request)
{
    // Init
    public_folder publicFolder;
    publicFolder.publicFolder("public", "data/static_files");

    // Compute
    auto file = publicFolder.publicFile("/public/images/test.jpg");

    // Assert
    EXPECT_TRUE(file != nullptr);

    // Compute
    file = publicFolder.publicFile("/somewhere");

    // Assert
    EXPECT_EQ(file, nullptr);

    // Compute
    file = publicFolder.publicFile("/publico/something");

    // Assert
    EXPECT_EQ(file, nullptr);
}

TEST(PublicFolder, Should_be_protected_against_directory_traversal_attacks)
{
    // Init
    public_folder publicFolder;
    publicFolder.publicFolder("public", "data/static_files");

    // Compute
    auto file = publicFolder.publicFile("/public/../read_test.txt");

    // Assert
    EXPECT_EQ(file, nullptr);
}

TEST(PublicFolder, Get_file_header_should_be_empty_if_wrong_path)
{
    // Init
    public_folder publicFolder;
    publicFolder.publicFolder("public", "data/static_files");

    // Compute
    auto file = publicFolder.publicFile("/public/eazeazdqsdazeazdazeazeaz.txt");

    // Assert
    EXPECT_EQ(file, nullptr);
}
