/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "request_matcher.h"

RequestMatcher::RequestMatcher(const std::string &header, const std::string &body)
{
    m_requestToMatch.setHeader(header);
    if (body.size())
        m_requestToMatch.setBody(body);
}

bool RequestMatcher::MatchAndExplain(const cpphttp::request::request &req,
                                     MatchResultListener * /* listener */) const
{
    return req == m_requestToMatch;
}

void RequestMatcher::DescribeTo(std::ostream *os) const
{
    *os << "is the expected request";
}

void RequestMatcher::DescribeNegationTo(std::ostream *os) const
{
    *os << "is not the expected request";
}

Matcher<const cpphttp::request::request &> SameRequest(const std::string &header, const std::string &body)
{
    return MakeMatcher(new RequestMatcher(header, body));
};
