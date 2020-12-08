/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */

#include "public_folder.h"
#include "internal/tools/string.h"

#include <unordered_map>

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

const std::unordered_map<std::string_view, const std::string> contentTypes = {
    {"jpeg", "image/jpeg"}, {"jpg", "image/jpeg"}, {"png", "image/png"}, {"tiff", "image/tiff"}, {"tif", "image/tiff"}, {"gif", "image/gif"}};

void public_folder::setContentType(const request::request &req, response::response &res)
{
    auto splittedValues = split(req.header().getPath(), ".");
    auto type = contentTypes.find(splittedValues.back());
    if (type != contentTypes.cend())
        res.header().setContentType(type->second);
}

void public_folder::handlePublicFiles(const request::request &req, response::response &res, std::string &errorValue) const noexcept
{
    std::string fullFile = readFile(extractPathFromRequest(req));
    if (fullFile.empty())
    {
        errorValue = MISSING_FILE;
        return;
    }
    setContentType(req, res);
    res.send(fullFile);
    res.end();
}

bool public_folder::isPublicFolderRequest(const std::string &path) const noexcept
{
    return std::regex_search(path, m_publicFolderRegex, std::regex_constants::match_continuous);
}

void public_folder::setRegex(const std::string &path) noexcept
{
    m_regexPath = path;
    if (path[path.size() - 1] != '/')
        m_regexPath += "/";
    m_publicFolderRegex = std::regex(m_regexPath);
}

std::string public_folder::extractPathFromRequest(const request::request &req) const noexcept
{
    std::string extractedFilePath = m_publicFolder + "/";
    std::string_view endOfPath = req.header().getPath();
    extractedFilePath += endOfPath.substr(m_regexPath.size());
    return extractedFilePath;
}
