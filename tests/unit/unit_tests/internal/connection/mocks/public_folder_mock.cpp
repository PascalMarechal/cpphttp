/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "public_folder_mock.h"

void PublicFolderMock::createFakeGetPublicFile()
{
    ON_CALL(*this, publicFile).WillByDefault(testing::Return(ExpectedFakeFile));
}

void PublicFolderMock::createFakeGetMissingPublicFile()
{
    ON_CALL(*this, publicFile).WillByDefault(testing::Return(nullptr));
}

PublicFileMock::PublicFileMock(std::string value) : m_value(std::move(value))
{
}

std::string PublicFileMock::header() const noexcept
{
    return m_value;
}
