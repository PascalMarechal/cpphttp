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

TEST(Public_Folder, Set_public_folder_will_always_set_absolute_path)
{
    public_folder publicFolder;
    publicFolder.setPublicFolder("public", "./relative");
    auto folder = publicFolder.getPublicFolder();
    std::filesystem::path cwd = std::filesystem::current_path();
    EXPECT_EQ(folder, cwd.string() + "/relative");

    publicFolder.setPublicFolder("public", "/absolute/path");
    folder = publicFolder.getPublicFolder();
    EXPECT_EQ(folder, "/absolute/path");
}

TEST(Public_Folder, Set_empty_public_folder_should_throw)
{
    public_folder publicFolder;
    EXPECT_THROW(publicFolder.setPublicFolder("", "./relative"), std::invalid_argument);
    EXPECT_THROW(publicFolder.setPublicFolder("somewhere", ""), std::invalid_argument);
}

TEST(Public_Folder, Check_if_request_is_a_public_folder_request)
{
    public_folder publicFolder;
    publicFolder.setPublicFolder("public", "data/static_files");
    EXPECT_TRUE(publicFolder.isPublicFolderRequest("public/jpg_test.jpg"));
    EXPECT_FALSE(publicFolder.isPublicFolderRequest("somewhere"));
    EXPECT_FALSE(publicFolder.isPublicFolderRequest("publico/something"));
}

TEST(Public_Folder, Get_jpg_from_public_folder)
{
    public_folder publicFolder;
    publicFolder.setPublicFolder("public", "data/static_files");
    auto request = Requests::GetRequestFromPath("public/jpg_test.jpg");
    cpphttp::response::response res;
    std::string error;
    publicFolder.handlePublicFiles(*request, res, error);
    EXPECT_THAT(res.toString(), HasSubstr("Content-Type:image/jpeg"));
}
