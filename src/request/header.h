#pragma once

#include <string>
#include "method.h"
#include "version.h"

#define MAX_HEADER_SIZE 8 * 1024

namespace cpphttp
{
    namespace request
    {
        class header
        {
        public:
            header(const std::string& data);

            bool isReady() const noexcept;
            method getMethod() const noexcept;
            version getVersion() const noexcept;
            const std::string &getPath() const noexcept;
            uint32_t getExpectedBodySize() const noexcept;

            void setPath(std::string_view path) noexcept;
            void setExpectedBodySize(uint32_t size) noexcept;

        private:
            inline void parseMethodValue(std::string_view value) noexcept;
            inline void parseVersionValue(std::string_view value) noexcept;
            inline void parseRequestLine(std::string_view line) noexcept;
            inline void handleHeaderLine(std::string_view line) noexcept;
            inline void parse(const std::string& data) noexcept;

            bool m_ready;
            method m_method;
            version m_version;
            uint32_t m_expectedBodysize;

            std::string m_path;
        };
    } // namespace request
} // namespace cpphttp
