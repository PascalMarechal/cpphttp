/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */

#include "public_folder.h"

using namespace cpphttp::internal;

void public_folder::publicFolder(const std::string &path, const std::string &folderPath)
{
    if (path.empty())
        throw std::invalid_argument("Missing path value in setPublicFolder");

    if (folderPath.empty())
        throw std::invalid_argument("Missing folder path value in setPublicFolder");

    auto folder = std::filesystem::absolute(folderPath).lexically_normal();
    m_publicFolderPath = folder.string();
    regex(path);
}

const std::string &public_folder::publicFolderPath() const noexcept
{
    return m_publicFolderPath;
}

bool public_folder::isPublicFolderRequest(const std::string &path) const noexcept
{
    return std::regex_search(path, m_publicFolderRegex, std::regex_constants::match_continuous);
}

inline void public_folder::regex(const std::string &path) noexcept
{
    m_publicFolderURL = path;
    if (path[path.size() - 1] != '/')
        m_publicFolderURL += "/";
    if (path[0] != '/')
        m_publicFolderURL = "/" + m_publicFolderURL;
    m_publicFolderRegex = std::regex(m_publicFolderURL);
}

inline void public_folder::sanitizeExtractedFilePathFromRequest(std::string &path) const noexcept
{
    path = std::filesystem::absolute(path).lexically_normal();
    auto matchFolderPath = path.find(m_publicFolderPath) == 0;
    if (!matchFolderPath)
        path = "";
}

inline std::string public_folder::extractFilePath(const std::string &urlPath) const noexcept
{
    std::string endOfPath = urlPath.substr(m_publicFolderURL.size());
    return m_publicFolderPath + "/" + endOfPath;
}

inline std::string public_folder::extractFilePathFromURL(const std::string &url) const noexcept
{
    auto extractedPath = extractFilePath(url);
    sanitizeExtractedFilePathFromRequest(extractedPath);
    return extractedPath;
}

inline std::string public_folder::getFilePathIfExists(const std::string &url) const noexcept
{
    if (!isPublicFolderRequest(url))
        return "";
    return extractFilePathFromURL(url);
}

const std::string &public_folder::publicFolderURL() const noexcept
{
    return m_publicFolderURL;
}

std::unique_ptr<public_file> public_folder::publicFile(const std::string &url) const noexcept
{
    auto path = getFilePathIfExists(url);
    if (path.empty())
        return nullptr;
    auto result = std::make_unique<public_file>(path);
    if (result->size() == std::numeric_limits<size_t>::max())
        return nullptr;
    return result;
}
