/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once
#include <gmock/gmock.h>

class PublicFileMock
{
public:
    PublicFileMock(std::string value);
    std::string header() const noexcept;

private:
    std::string m_value;
};

class PublicFolderMock
{
public:
    static inline std::string ExpectedFakeResult = "Some File Header Data...";
    static inline std::shared_ptr<PublicFileMock> ExpectedFakeFile = std::make_shared<PublicFileMock>(ExpectedFakeResult);
    
    MOCK_CONST_METHOD1(publicFile, std::shared_ptr<PublicFileMock>(const std::string &));

    void createFakeGetPublicFile();
    void createFakeGetMissingPublicFile();
};
