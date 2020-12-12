/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once
#include <gmock/gmock.h>

class PublicFolderMock
{
public:
    static inline std::string ExpectedFakeResult = "Some File Header Data...";

    MOCK_CONST_METHOD1(getFilePathIfExists, std::string(const std::string &));
    MOCK_CONST_METHOD1(getFileHeader, std::string(const std::string &));
    
    void createFakeFilePathDoesNotExist();
    void createFakeFilePathDoesExist(const std::string &path);
    void createFakeGetFileHeader();
    void createFakeGetMissingFileHeader();
};
