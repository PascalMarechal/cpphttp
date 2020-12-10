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
    {"jpeg", "image/jpeg"}, {"jpg", "image/jpeg"}, {"png", "image/png"}, {"tiff", "image/tiff"}, {"tif", "image/tiff"}, {"gif", "image/gif"}, {"svg", "image/svg+xml"}, {"svgz", "image/svg+xml"}, {"ico", "image/x-icon"}, {"js", "application/javascript"}, {"json", "application/json"}, {"pdf", "application/pdf"}, {"css", "text/css"}, {"csv", "text/csv"}, {"html", "text/html; charset=UTF-8"}, {"xml", "text/xml"}};

inline void public_folder::setContentType(const request::request &req, response::response &res) noexcept
{
    auto splittedValues = split(req.header().getPath(), ".");
    auto type = contentTypes.find(splittedValues.back());
    if (type != contentTypes.cend())
        res.header().setContentType(type->second);
    else
        res.header().setContentType("text/plain");
}

void public_folder::handlePublicFiles(const request::request &req, response::response &res, std::string &errorValue) const noexcept
{
    auto filePath = extractFilePathFromRequest(req);
    if (filePath.empty())
    {
        errorValue = MISSING_FILE;
        return;
    }

    auto fullFile = readBinaryFile(filePath);
    if (fullFile.size() == 0)
    {
        errorValue = MISSING_FILE;
        return;
    }
    setContentType(req, res);
    res.send(std::move(fullFile));
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
    if (path[0] != '/')
        m_regexPath = "/" + m_regexPath;
    m_publicFolderRegex = std::regex(m_regexPath);
}

inline void public_folder::sanitizeExtractedFilePathFromRequest(std::string &path) const noexcept
{
    path = std::filesystem::absolute(path).lexically_normal();
    auto matchFolderPath = path.find(m_publicFolder) == 0;
    if (!matchFolderPath)
        path = "";
}

inline std::string public_folder::extractFilePath(const std::string &urlPath) const noexcept
{
    std::string endOfPath = urlPath.substr(m_regexPath.size());
    return m_publicFolder + "/" + endOfPath;
}

inline std::string public_folder::extractFilePathFromRequest(const request::request &req) const noexcept
{
    auto extractedPath = extractFilePath(req.header().getPath());
    sanitizeExtractedFilePathFromRequest(extractedPath);
    return extractedPath;
}

inline std::string public_folder::extractFilePathFromURL(const std::string &url) const noexcept
{
    auto extractedPath = extractFilePath(url);
    sanitizeExtractedFilePathFromRequest(extractedPath);
    return extractedPath;
}

std::string public_folder::getFilePathIfExists(const std::string &url)
{
    if (!isPublicFolderRequest(url))
        return "";
    return extractFilePathFromURL(url);
}
