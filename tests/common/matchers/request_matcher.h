#pragma once

#include <gmock/gmock.h>
#include "common/requests.h"
#include "request/request.h"

using ::testing::MakeMatcher;
using ::testing::Matcher;
using ::testing::MatcherInterface;
using ::testing::MatchResultListener;

class RequestMatcher : public MatcherInterface<const cpphttp::request::request &>
{
public:
    RequestMatcher(const std::string &header, const std::string &body);

    bool MatchAndExplain(const cpphttp::request::request &req,
                         MatchResultListener * /* listener */) const override;

    void DescribeTo(std::ostream *os) const override;

    void DescribeNegationTo(std::ostream *os) const override;
private:
    cpphttp::request::request m_requestToMatch;
};

Matcher<const cpphttp::request::request &> SameRequest(const std::string &header, const std::string &body);
