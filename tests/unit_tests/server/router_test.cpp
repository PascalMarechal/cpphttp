#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "server/router.h"
#include "common/requests.h"

using namespace cpphttp::server;
using namespace cpphttp::request;
using ::testing::HasSubstr;

std::unique_ptr<request> getCorrectPostRequest()
{
    auto req = std::make_unique<request>();
    req->setHeader(Requests::PostRequestHeader);
    req->setBody(Requests::PostRequestBody);
    return req;
}

TEST(Router, ShouldThrowWithInvalidRequest)
{
    router router;
    request req;
    EXPECT_THROW(router.process(req), std::invalid_argument);
}

TEST(Router, ShouldNotThrowWithValidRequest)
{
    router router;
    auto req = getCorrectPostRequest();
    EXPECT_NO_THROW(router.process(*req));
}

TEST(Router, ShouldHaveDefaultInternalErrorResponse)
{
    router router;
    auto req = getCorrectPostRequest();
    EXPECT_THAT(router.process(*req), HasSubstr("500 Internal Server Error"));
}
