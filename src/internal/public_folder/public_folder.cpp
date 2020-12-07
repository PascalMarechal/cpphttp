/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */

#include "public_folder.h"

using namespace cpphttp::internal;

void public_folder::setPublicFolder(const std::string &path, const std::string &folderPath)
{
    if (path.empty())
        throw std::invalid_argument("Missing path value in setPublicFolder");

    if (folderPath.empty())
        throw std::invalid_argument("Missing folder path value in setPublicFolder");

    auto folder = std::filesystem::absolute(folderPath).lexically_normal();
    m_publicFolder = folder.string();
    setRegex(path);
}

const std::string &public_folder::getPublicFolder() const noexcept
{
    return m_publicFolder;
}

void public_folder::handlePublicFiles(request::request &req, response::response &res, std::string &errorValue) const noexcept
{
    res.header().setContentType("image/jpeg");
    res.end();
}

bool public_folder::isPublicFolderRequest(const std::string &path) const noexcept
{
    return std::regex_search(path, m_publicFolderRegex, std::regex_constants::match_continuous);
}

void public_folder::setRegex(const std::string &path) noexcept
{
    if (path[path.size() - 1] != '/')
        m_publicFolderRegex = std::regex(path + "/");
    else
        m_publicFolderRegex = std::regex(path);
}
