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
    // Init
    public_folder publicFolder;
    std::filesystem::path cwd = std::filesystem::current_path();

    // Compute
    publicFolder.setPublicFolder("public", "./relative");
    auto folder = publicFolder.getPublicFolder();

    // Assert
    EXPECT_EQ(folder, cwd.string() + "/relative");

    // Compute
    publicFolder.setPublicFolder("public", "/absolute/path");
    folder = publicFolder.getPublicFolder();

    // Assert
    EXPECT_EQ(folder, "/absolute/path");
}

TEST(Public_Folder, Set_empty_public_folder_should_throw)
{
    // Init
    public_folder publicFolder;

    // Assert
    EXPECT_THROW(publicFolder.setPublicFolder("", "./relative"), std::invalid_argument);
    EXPECT_THROW(publicFolder.setPublicFolder("somewhere", ""), std::invalid_argument);
}

TEST(Public_Folder, Check_if_request_is_a_public_folder_request)
{
    // Init
    public_folder publicFolder;

    // Compute
    publicFolder.setPublicFolder("public", "data/static_files");

    // Assert
    EXPECT_TRUE(publicFolder.isPublicFolderRequest("public/test.jpg"));
    EXPECT_FALSE(publicFolder.isPublicFolderRequest("somewhere"));
    EXPECT_FALSE(publicFolder.isPublicFolderRequest("publico/something"));

    // Compute
    publicFolder.setPublicFolder("public/", "data/static_files");

    // Assert
    EXPECT_TRUE(publicFolder.isPublicFolderRequest("public/image.jpg"));
}

TEST(Public_Folder, Missing_file_should_not_touch_the_response_and_should_return_error)
{
    // Init
    public_folder publicFolder;
    publicFolder.setPublicFolder("public", "data/static_files");
    auto request = Requests::GetRequestFromPath("public/not_there.jpg");
    cpphttp::response::response res;
    std::string error;
    auto resBefore = res.toString();

    // Compute
    publicFolder.handlePublicFiles(*request, res, error);

    // Assert
    EXPECT_FALSE(res.hasEnded());
    EXPECT_EQ(res.toString(), resBefore);
    EXPECT_EQ(error, public_folder::MISSING_FILE);
}

void formatTest(std::string file, std::string expectedFormat)
{
    // Init
    public_folder publicFolder;
    publicFolder.setPublicFolder("public", "data/static_files");
    auto request = Requests::GetRequestFromPath("public/" + file);
    auto expectedFileData = readFile("data/static_files/" + file);
    cpphttp::response::response res;
    std::string error;

    // Compute
    publicFolder.handlePublicFiles(*request, res, error);
    auto result = res.toString();

    // Assert
    EXPECT_THAT(result, HasSubstr("Content-Type:" + expectedFormat));
    EXPECT_FALSE(expectedFileData.empty());
    EXPECT_THAT(result, HasSubstr(expectedFileData));
}

TEST(Public_Folder, Get_jpg_from_public_folder)
{
    formatTest("images/test.jpg", "image/jpeg");
}

TEST(Public_Folder, Get_jpeg_from_public_folder)
{
    formatTest("images/test.jpeg", "image/jpeg");
}

TEST(Public_Folder, Get_png_from_public_folder)
{
    formatTest("images/test.png", "image/png");
}

TEST(Public_Folder, Get_tiff_from_public_folder)
{
    formatTest("images/test.tiff", "image/tiff");
}

TEST(Public_Folder, Get_tif_from_public_folder)
{
    formatTest("images/test.tif", "image/tiff");
}

TEST(Public_Folder, Get_gif_from_public_folder)
{
    formatTest("images/test.gif", "image/gif");
}

TEST(Public_Folder, Get_svg_from_public_folder)
{
    formatTest("images/test.svg", "image/svg+xml");
}

TEST(Public_Folder, Get_svgz_from_public_folder)
{
    formatTest("images/test.svgz", "image/svg+xml");
}

TEST(Public_Folder, Get_ico_from_public_folder)
{
    formatTest("images/test.ico", "image/x-icon");
}
