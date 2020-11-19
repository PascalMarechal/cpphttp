#pragma once

#include "status.h"
#include "header.h"
#include <memory>

namespace cpphttp
{
    namespace response
    {
        class response
        {
        public:
            response();
            ~response();
            void status(cpphttp::response::status) noexcept;
            std::string toString() const noexcept;
            void write(const std::string &data) noexcept;
            void send(const std::string &data) noexcept;
            void end() noexcept;
            bool hasEnded() const noexcept;

        private:
            class impl;
            std::unique_ptr<impl> m_impl;
        };
    } // namespace response
} // namespace cpphttp