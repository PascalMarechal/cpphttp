#pragma once

#include <string>
#include "method.h"
#include "version.h"

namespace cpphttp
{
    namespace request
    {
        class header
        {
        public:
            header();

            std::string read(const std::string &data) noexcept;

            bool isReady() noexcept;
            method getMethod() noexcept;
            version getVersion() noexcept;
            std::string getPath() noexcept;

            void setPath(std::string path) noexcept;

        private:
            inline void parseMethodValue(const std::string &value) noexcept;
            inline void parseVersionValue(const std::string &value) noexcept;
            inline void parseRequestLine(const std::string &line) noexcept;
            inline void handleHeaderLine(const std::string &line) noexcept;

            static inline std::size_t findEndOfHeaderData(const std::string &data) noexcept;
            inline void appendRawData(const std::string &data, std::size_t to) noexcept;
            inline void parse() noexcept;

            bool m_ready;
            method m_method;
            version m_version;
            std::string m_path;

            std::string m_rawData;
        };
    } // namespace request
} // namespace cpphttp
