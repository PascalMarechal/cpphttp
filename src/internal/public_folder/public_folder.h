/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include "request/request.h"
#include "response/response.h"
#include "public_file.h"

#include <regex>
#include <filesystem>

namespace cpphttp
{
    namespace internal
    {
        class public_folder
        {
        public:
            void publicFolder(const std::string &path, const std::string &folderPath);
            const std::string &publicFolderPath() const noexcept;
            const std::string &publicFolderURL() const noexcept;
            std::unique_ptr<public_file> publicFile(const std::string &url) const noexcept;

        private:
            std::string m_publicFolderPath;
            std::string m_publicFolderURL;
            std::regex m_publicFolderRegex;

            inline void setRegex(const std::string &path) noexcept;
            inline bool isPublicFolderRequest(const std::string &path) const noexcept;
            inline std::string extractFilePathFromURL(const std::string &url) const noexcept;
            inline std::string extractFilePath(const std::string &urlPath) const noexcept;
            inline void sanitizeExtractedFilePathFromRequest(std::string &path) const noexcept;
            inline std::string getFilePathIfExists(const std::string &url) const noexcept;
        };
    } // namespace internal
} // namespace cpphttp
