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
            static const inline std::string MISSING_FILE = "File is missing/Wrong path";

            void setPublicFolder(const std::string &path, const std::string &folderPath);
            const std::string &getPublicFolder() const noexcept;
            void handlePublicFiles(request::request &req, response::response &res, std::string &errorValue) const noexcept;
            bool isPublicFolderRequest(const std::string &path) const noexcept;

        private:
            std::string m_publicFolder;
            std::string m_regexPath;
            std::regex m_publicFolderRegex;

            void setRegex(const std::string &path) noexcept;
            std::string extractPathFromRequest(const request::request &req) const noexcept;
        };
    } // namespace internal
} // namespace cpphttp
