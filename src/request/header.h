#pragma once

#include <string>
#include "method.h"

namespace cpphttp
{
    namespace request
    {
        class header
        {
        public:
            header();
            bool isReady();
            std::string fill(const std::string &data);

        private:
            void parseRequestLine(const std::string &line);
            void handleHeaderLine(const std::string &line);
            
            bool m_isReady;
            method m_method;
        };
    } // namespace request
} // namespace cpphttp
