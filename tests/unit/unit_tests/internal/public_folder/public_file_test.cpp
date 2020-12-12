/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal/public_folder/public_file.h"
#include "internal/tools/string.h"

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

void formatTest(std::string file, std::string expectedFormat)
{
    // Init
    std::string filePath = "data/static_files/" + file;
    public_file publicFile(filePath);
    auto expectedFileData = readFile(filePath);

    // Compute
    auto result = publicFile.header();

    // Assert
    EXPECT_THAT(result, HasSubstr("Content-Type:" + expectedFormat));
    EXPECT_FALSE(expectedFileData.empty());
}

TEST(PublicFile, Unknown_types_should_be_plain_text)
{
    formatTest("text/test.unknown", "text/plain");
    formatTest("text/test.unknown2", "text/plain");
}

// Images

TEST(PublicFile, Get_jpg_from_public_folder)
{
    formatTest("images/test.jpg", "image/jpeg");
}

TEST(PublicFile, Get_jpeg_from_public_folder)
{
    formatTest("images/test.jpeg", "image/jpeg");
}

TEST(PublicFile, Get_png_from_public_folder)
{
    formatTest("images/test.png", "image/png");
}

TEST(PublicFile, Get_tiff_from_public_folder)
{
    formatTest("images/test.tiff", "image/tiff");
}

TEST(PublicFile, Get_tif_from_public_folder)
{
    formatTest("images/test.tif", "image/tiff");
}

TEST(PublicFile, Get_gif_from_public_folder)
{
    formatTest("images/test.gif", "image/gif");
}

TEST(PublicFile, Get_svg_from_public_folder)
{
    formatTest("images/test.svg", "image/svg+xml");
}

TEST(PublicFile, Get_svgz_from_public_folder)
{
    formatTest("images/test.svgz", "image/svg+xml");
}

TEST(PublicFile, Get_ico_from_public_folder)
{
    formatTest("images/test.ico", "image/x-icon");
}

// Applications

TEST(PublicFile, Get_js_from_public_folder)
{
    formatTest("applications/test.js", "application/javascript");
}

TEST(PublicFile, Get_json_from_public_folder)
{
    formatTest("applications/test.json", "application/json");
}

TEST(PublicFile, Get_pdf_from_public_folder)
{
    formatTest("applications/test.pdf", "application/pdf");
}

// Text

TEST(PublicFile, Get_css_from_public_folder)
{
    formatTest("text/test.css", "text/css");
}

TEST(PublicFile, Get_csv_from_public_folder)
{
    formatTest("text/test.csv", "text/csv");
}

TEST(PublicFile, Get_html_from_public_folder)
{
    formatTest("text/test.html", "text/html; charset=UTF-8");
}

TEST(PublicFile, Get_xml_from_public_folder)
{
    formatTest("text/test.xml", "text/xml");
}
