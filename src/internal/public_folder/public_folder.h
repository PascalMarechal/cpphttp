/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include "request/request.h"
#include "response/response.h"

#include <regex>
#include <filesystem>

namespace cpphttp
{
    namespace internal
    {
        class public_folder
        {
        public:
            void setPublicFolder(const std::string &path, const std::string &folderPath);
            const std::string &getPublicFolder() const noexcept;
            std::string getFilePathIfExists(const std::string &url) const noexcept;
            std::string getFileHeader(const std::string &path) const noexcept;

        private:
            std::string m_publicFolder;
            std::string m_regexPath;
            std::regex m_publicFolderRegex;

            void setRegex(const std::string &path) noexcept;
            inline bool isPublicFolderRequest(const std::string &path) const noexcept;
            inline std::string extractFilePathFromURL(const std::string &url) const noexcept;
            inline std::string extractFilePath(const std::string &urlPath) const noexcept;
            inline void sanitizeExtractedFilePathFromRequest(std::string &path) const noexcept;
            inline static void setContentType(const std::string &path, cpphttp::response::header &head) noexcept;
        };
    } // namespace internal
} // namespace cpphttp
