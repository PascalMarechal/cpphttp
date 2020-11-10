#pragma once

#include "internal/asio.h"
namespace cpphttp
{
    namespace internal
    {
        class match_end_of_header
        {
        public:
            template <typename Iterator>
            std::pair<Iterator, bool> operator()(
                Iterator begin, Iterator end) const
            {
                Iterator i = begin;
                while (i != end)
                    if (c_ == *i++)
                        return std::make_pair(i, true);
                return std::make_pair(i, false);
            }

        private:
            char c_ = 'm';
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
