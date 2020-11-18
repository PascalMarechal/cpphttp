#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "server/router.h"
#include "common/requests.h"

using namespace cpphttp::server;
using namespace cpphttp::request;
using namespace cpphttp::response;

using ::testing::HasSubstr;

std::unique_ptr<request> getCorrectPostRequest()
{
    auto req = std::make_unique<request>();
    req->setHeader(Requests::PostRequestHeader);
    req->setBody(Requests::PostRequestBody);
    return req;
}

auto postRequest = getCorrectPostRequest();

TEST(Router, ShouldThrowWithInvalidRequest)
{
    router router;
    request req;
    EXPECT_THROW(router.process(req), std::invalid_argument);
}

TEST(Router, ShouldNotThrowWithValidRequest)
{
    router router;
    EXPECT_NO_THROW(router.process(*postRequest));
}

TEST(Router, ShouldHaveDefaultInternalErrorResponse)
{
    router router;
    EXPECT_THAT(router.process(*postRequest), HasSubstr("500 Internal Server Error"));
}

TEST(Router, ShouldBeAbleToUseOtherErrorFunctions)
{
    router router;
    auto errorFunction = [](const std::string &error, request &req, response &res) {
        res.status(status::_501);
        res.send("<h1> Error !!! </h1>");
    };
    router.error(errorFunction);
    EXPECT_THAT(router.process(*postRequest), HasSubstr("501 Not Implemented"));
    EXPECT_THAT(router.process(*postRequest), HasSubstr("<h1> Error !!! </h1>"));
}

TEST(Router, ShouldBeAbleToUseOtherErrorFunctionsInCascade)
{
    router router;
    auto errorFunction = [](const std::string &error, request &req, response &res) {
        res.status(status::_501);
    };
    auto errorFunction2 = [](const std::string &error, request &req, response &res) {
        res.send("<h1>Whoops</h1>");
    };
    router.error(errorFunction);
    router.error(errorFunction2);
    auto result = router.process(*postRequest);
    EXPECT_THAT(result, HasSubstr("501 Not Implemented"));
    EXPECT_THAT(result, HasSubstr("<h1>Whoops</h1>"));
}

TEST(Router, ShouldStopErrorFunctionsAtFirstSend)
{
    router router;
    auto errorFunction = [](const std::string &error, request &req, response &res) {
        res.status(status::_501);
        res.send("<h1>Something</h1>");
    };
    auto errorFunction2 = [](const std::string &error, request &req, response &res) {
        res.status(status::_500);
    };
    router.error(errorFunction);
    router.error(errorFunction2);
    auto result = router.process(*postRequest);
    EXPECT_THAT(result, HasSubstr("501 Not Implemented"));
}

TEST(Router, ShouldHaveVariadicErrorFunctionSetter)
{
    router router;
    auto errorFunction = [](const std::string &error, request &req, response &res) {
        res.status(status::_501);
    };
    auto errorFunction2 = [](const std::string &error, request &req, response &res) {
        res.write("<h1>Whoops</h1>");
    };
    auto errorFunction3 = [](const std::string &error, request &req, response &res) {
        res.send("<h2>Heyyyy</h2>");
    };
    router.error(errorFunction, errorFunction2, errorFunction3);
    auto result = router.process(*postRequest);
    EXPECT_THAT(result, HasSubstr("501 Not Implemented"));
    EXPECT_THAT(result, HasSubstr("<h1>Whoops</h1>"));
    EXPECT_THAT(result, HasSubstr("<h2>Heyyyy</h2>"));
}
