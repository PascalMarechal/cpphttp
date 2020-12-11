#include "public_folder_mock.h"

void PublicFolderMock::createFakeFilePathDoesNotExist()
{
    ON_CALL(*this, getFilePathIfExists).WillByDefault(testing::Return(""));
}

void PublicFolderMock::createFakeFilePathDoesExist(const std::string &path)
{
    ON_CALL(*this, getFilePathIfExists).WillByDefault([path](const std::string &) { return path; });
}

void PublicFolderMock::createFakeGetFileHeader()
{
    ON_CALL(*this, getFileHeader).WillByDefault(testing::Return(ExpectedFakeResult));
}
