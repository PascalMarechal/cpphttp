/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once

#include "internal/asio.h"
namespace cpphttp
{
    namespace internal
    {
        class match_end_of_header
        {
        public:
            match_end_of_header() : m_maxHeaderSize(std::numeric_limits<uint64_t>::max())
            {
            }

            template <typename Iterator>
            std::pair<Iterator, bool> operator()(
                Iterator begin, Iterator end) const noexcept
            {
                if (std::distance(begin, end) >= m_maxHeaderSize)
                    return std::make_pair(begin, true);

                Iterator i = begin;
                while (true)
                    // \n\n => OK
                    if (checkCurrent('\n', i, end) && checkNext('\n', i, end))
                        return std::make_pair(++i, true);
                    // \r\n => Check for \n or \r\n
                    else if ((checkCurrent('\r', i, end) && checkNext('\n', i, end))
                             // \n or \r\n
                             && (checkNext('\n', i, end) || (checkCurrent('\r', i, end) && checkNext('\n', i, end))))
                        return std::make_pair(++i, true);
                    else if (i == end)
                        break;
                    else if (!checkCurrent('\r', i, end))
                        ++i;

                return std::make_pair(i, false);
            }

            void setMaxHeaderSize(uint64_t size) noexcept
            {
                m_maxHeaderSize = size;
            }

        private:
            template <typename Iterator>
            inline bool checkNext(char c, Iterator &current, Iterator end) const noexcept
            {
                return current++ != end && *current == c;
            }

            template <typename Iterator>
            inline bool checkCurrent(char c, Iterator current, Iterator end) const noexcept
            {
                return current != end && *current == c;
            }

            uint64_t m_maxHeaderSize;
        };
    } // namespace internal
} // namespace cpphttp

namespace asio
{
    template <>
    struct is_match_condition<cpphttp::internal::match_end_of_header>
        : public true_type
    {
    };
} // namespace asio
