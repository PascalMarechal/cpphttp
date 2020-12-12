/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "public_file.h"
#include "internal/tools/string.h"

#include <sys/stat.h>
#include <unordered_map>

using namespace cpphttp::internal;

public_file::public_file(std::string path) : m_path(std::move(path))
{
    initFileSize();
}

const std::string &public_file::path() noexcept
{
    return m_path;
}

size_t public_file::size() noexcept
{
    return m_size;
}

std::string public_file::header() noexcept
{
    if (m_size == std::numeric_limits<size_t>::max())
        return "";

    cpphttp::response::header head;
    head.contentLength(m_size);
    contentType(head);
    return head.toString();
}

inline void public_file::initFileSize() noexcept
{
    struct stat statbuf;
    if (stat(m_path.c_str(), &statbuf) != 0)
        m_size = std::numeric_limits<size_t>::max();
    else
        m_size = statbuf.st_size;
}

const std::unordered_map<std::string_view, const std::string> contentTypes = {
    {"jpeg", "image/jpeg"}, {"jpg", "image/jpeg"}, {"png", "image/png"}, {"tiff", "image/tiff"}, {"tif", "image/tiff"}, {"gif", "image/gif"}, {"svg", "image/svg+xml"}, {"svgz", "image/svg+xml"}, {"ico", "image/x-icon"}, {"js", "application/javascript"}, {"json", "application/json"}, {"pdf", "application/pdf"}, {"css", "text/css"}, {"csv", "text/csv"}, {"html", "text/html; charset=UTF-8"}, {"xml", "text/xml"}};

inline void public_file::contentType(cpphttp::response::header &head) noexcept
{
    auto splittedValues = split(m_path, ".");
    auto type = contentTypes.find(splittedValues.back());
    if (type != contentTypes.cend())
        head.contentType(type->second);
    else
        head.contentType("text/plain");
}
