/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal/public_folder/public_folder.h"
#include "common/requests.h"
#include "internal/tools/string.h"

using namespace cpphttp::internal;
using ::testing::HasSubstr;

TEST(Public_Folder, Set_public_folder_will_always_set_absolute_path)
{
    public_folder publicFolder;
    std::filesystem::path cwd = std::filesystem::current_path();

    publicFolder.setPublicFolder("public", "./relative");

    auto folder = publicFolder.getPublicFolder();
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

    publicFolder.setPublicFolder("public/", "data/static_files");

    EXPECT_TRUE(publicFolder.isPublicFolderRequest("public/image.jpg"));
}

TEST(Public_Folder, Missing_file_should_not_touch_the_response_and_should_return_error)
{
    public_folder publicFolder;
    publicFolder.setPublicFolder("public", "data/static_files");
    auto request = Requests::GetRequestFromPath("public/not_there.jpg");
    cpphttp::response::response res;
    std::string error;
    auto resBefore = res.toString();

    publicFolder.handlePublicFiles(*request, res, error);

    EXPECT_FALSE(res.hasEnded());
    EXPECT_EQ(res.toString(), resBefore);
    EXPECT_EQ(error, public_folder::MISSING_FILE);
}

TEST(Public_Folder, Get_jpg_from_public_folder)
{
    public_folder publicFolder;
    publicFolder.setPublicFolder("public", "data/static_files");
    auto request = Requests::GetRequestFromPath("public/jpg_test.jpg");
    cpphttp::response::response res;
    std::string error;

    publicFolder.handlePublicFiles(*request, res, error);

    auto result = res.toString();
    EXPECT_THAT(result, HasSubstr("Content-Type:image/jpeg"));
    EXPECT_THAT(result, HasSubstr(readFile("data/static_files/jpg_test.jpg")));
}
