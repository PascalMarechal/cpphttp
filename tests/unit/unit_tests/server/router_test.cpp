/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>

#include "server/router.h"
#include "common/requests.h"

using namespace cpphttp::server;
using namespace cpphttp::request;
using namespace cpphttp::response;

using ::testing::HasSubstr;

std::string processToString(router &router, request &req)
{
    auto result = router.process(req);
    return std::string(result.cbegin(), result.cend());
}

TEST(Router, Should_throw_with_invalid_request)
{
    router router;
    request req;
    EXPECT_THROW(router.process(req), std::invalid_argument);
}

TEST(Router, Should_not_throw_with_valid_request)
{
    router router;
    EXPECT_NO_THROW(router.process(*Requests::PostRequest));
}

TEST(Router, Should_have_default_internal_error_response)
{
    router router;
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("500 Internal Server Error"));
}

TEST(Router, Should_be_able_to_use_other_error_functions)
{
    router router;
    auto errorFunction = [](const std::string &error, request &req, response &res) {
        res.status(status::_501);
        res.send("<h1> Error !!! </h1>");
    };
    router.onError(errorFunction);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("501 Not Implemented"));
    EXPECT_THAT(result, HasSubstr("<h1> Error !!! </h1>"));
}

TEST(Router, Should_be_able_to_use_other_error_functions_in_cascade)
{
    router router;
    auto errorFunction = [](const std::string &error, request &req, response &res) {
        res.status(status::_501);
    };
    auto errorFunction2 = [](const std::string &error, request &req, response &res) {
        res.send("<h1>Whoops</h1>");
    };
    router.onError(errorFunction);
    router.onError(errorFunction2);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("501 Not Implemented"));
    EXPECT_THAT(result, HasSubstr("<h1>Whoops</h1>"));
}

TEST(Router, Should_stop_error_functions_at_first_send)
{
    router router;
    auto errorFunction = [](const std::string &error, request &req, response &res) {
        res.status(status::_501);
        res.send("<h1>Something</h1>");
    };
    auto errorFunction2 = [](const std::string &error, request &req, response &res) {
        res.status(status::_500);
    };
    router.onError(errorFunction);
    router.onError(errorFunction2);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("501 Not Implemented"));
}

TEST(Router, Should_have_variadic_error_functions_setter)
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
    router.onError(errorFunction, errorFunction2, errorFunction3);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("501 Not Implemented"));
    EXPECT_THAT(result, HasSubstr("<h1>Whoops</h1>"));
    EXPECT_THAT(result, HasSubstr("<h2>Heyyyy</h2>"));
}

auto rootFunction = [](request &req, response &res, error_callback error) {
    res.send("Root Function Called");
};

TEST(Router, Should_be_able_to_use_functions_for_paths_starting_with)
{
    router router;
    router.onAll("/", rootFunction);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("Root Function Called"));
}

TEST(Router, Should_avoid_using_functions_with_wrong_path)
{
    router router;
    router.onAll("/wrongpath", rootFunction);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("500 Internal Server Error"));
}

auto firstFunction = [](request &req, response &res, error_callback error) {
    res.write("First Function Called");
};
auto secondFunction = [](request &req, response &res, error_callback error) {
    res.send("Second Function Called");
};

void testFirstAndSecondFunction(const std::string &result)
{
    EXPECT_THAT(result, HasSubstr("First Function Called"));
    EXPECT_THAT(result, HasSubstr("Second Function Called"));
}

TEST(Router, Should_be_able_to_use_functions_in_cascade)
{
    router router;
    router.onAll("/", firstFunction);
    router.onAll("/", secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    testFirstAndSecondFunction(result);
}

TEST(Router, Should_stop_using_functions_when_response_ended)
{
    router router;
    router.onAll("/", firstFunction);
    router.onAll("/", secondFunction);
    router.onAll("/", [](request &req, response &res, error_callback error) { res.status(status::_204); });
    auto result = processToString(router, *Requests::PostRequest);
    testFirstAndSecondFunction(result);
    EXPECT_THAT(result, Not(HasSubstr("204")));
}

TEST(Router, Should_avoid_using_functions_with_wrong_path_along_with_correct_one)
{
    router router;
    router.onAll("/wrongpath", firstFunction);
    router.onAll("/", secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, Not(HasSubstr("First Function Called")));
    EXPECT_THAT(result, HasSubstr("Second Function Called"));
}

TEST(Router, Should_switch_to_error_functions_when_triggered)
{
    router router;
    router.onAll("/", firstFunction);
    router.onAll("/", [](request &req, response &res, error_callback error) { error("something bad happened!"); });
    router.onAll("/", secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("500 Internal Server Error"));
}

TEST(Router, Should_switch_to_error_functions_when_triggered_without_text)
{
    router router;
    router.onAll("/", firstFunction);
    router.onAll("/", [](request &req, response &res, error_callback error) { error(""); });
    router.onAll("/", secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("500 Internal Server Error"));
}

TEST(Router, Error_functions_should_have_access_to_error_value)
{
    router router;
    router.onAll("/", [](request &req, response &res, error_callback error) { error("An error happened"); });
    router.onError([](const std::string &error, request &req, response &res) {
        res.send("<h1>" + error + "</h1>");
    });
    auto result = processToString(router, *Requests::PostRequest);
    EXPECT_THAT(result, HasSubstr("An error happened"));
}

TEST(Router, Should_use_regex_in_path_variable)
{
    router router;
    router.onAll("/pa(bc)?th", firstFunction);
    router.onAll(".*", secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    testFirstAndSecondFunction(result);
}

TEST(Router, Should_have_variadic_onAll_function)
{
    router router;
    router.onAll(
        "/", firstFunction, [](request &req, response &res, error_callback error) { res.write("Middle Function"); }, secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    testFirstAndSecondFunction(result);
    EXPECT_THAT(result, HasSubstr("Middle Function"));
}

auto getFunction = [](request &req, response &res, error_callback error) { res.send("Get function called"); };

TEST(Router, Should_have__onGet_function)
{
    router router;
    router.onGet("/index", getFunction);
    auto result = processToString(router, *Requests::GetRequest);
    EXPECT_THAT(result, HasSubstr("Get function called"));
}

TEST(Router, Get_functions_should_fail_if_path_is_different_or_incomplete)
{
    router router;
    router.onGet("/ind", getFunction);
    auto result = processToString(router, *Requests::GetRequest);
    EXPECT_THAT(result, Not(HasSubstr("Get function called")));
}

std::string processGetRootPath(router &router)
{
    request req;
    req.header("GET / HTTP/1.1 \n\n");
    return processToString(router, req);
}

TEST(Router, root_path_should_be_allowed)
{
    router router;
    router.onGet("/", getFunction);
    auto result = processGetRootPath(router);
    EXPECT_THAT(result, HasSubstr("Get function called"));
}

TEST(Router, empty_path_should_be_root_path)
{
    router router;
    router.onGet("", getFunction);
    auto result = processGetRootPath(router);
    EXPECT_THAT(result, HasSubstr("Get function called"));
}

TEST(Router, Get_functions_should_match_with_param_in_url)
{
    router router;
    router.onGet("/item/:id", getFunction);
    auto result = processToString(router, *Requests::GetRequestWithParam);
    EXPECT_THAT(result, HasSubstr("Get function called"));
}

TEST(Router, Get_functions_should_have_access_to_param_in_url)
{
    router router;
    router.onGet("/item/:id", [](request &req, response &res, error_callback error) { res.send("ID value is " + req.getParam("id")); });
    router.onGet("/complex/:index/:text/url", [](request &req, response &res, error_callback error) { res.send("Index value is " + req.getParam("index") + "\nText value is " + req.getParam("text")); });
    auto result = processToString(router, *Requests::GetRequestWithParam);
    EXPECT_THAT(result, HasSubstr("ID value is 13"));
    result = processToString(router, *Requests::GetRequestWithTailingParams);
    EXPECT_THAT(result, HasSubstr("Index value is 13\nText value is hello"));
}

TEST(Router, Should_have_variadic_onGet_function)
{
    router router;
    router.onGet("/item/:id", firstFunction, secondFunction);
    auto result = processToString(router, *Requests::GetRequestWithParam);
    testFirstAndSecondFunction(result);
}

TEST(Router, Should_have_onPost_function)
{
    router router;
    router.onPost("/path/somewhere", firstFunction);
    router.onPost("/path/somewhere", secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    testFirstAndSecondFunction(result);
}

void expectFirstAndSecondFunctionNotCalled(const std::string &input)
{
    EXPECT_THAT(input, Not(HasSubstr("First Function Called")));
    EXPECT_THAT(input, Not(HasSubstr("Second Function Called")));
}

TEST(Router, Functions_should_not_be_mixed)
{
    router router;
    router.onGet("/path/somewhere", firstFunction, secondFunction);
    router.onPost("/item/:id", firstFunction);
    router.onPost("/item/:id", secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    expectFirstAndSecondFunctionNotCalled(result);
    result = processToString(router, *Requests::GetRequestWithParam);
    expectFirstAndSecondFunctionNotCalled(result);
}

TEST(Router, Should_have_variadic_onPost_function)
{
    router router;
    router.onPost("/path/somewhere", firstFunction, secondFunction);
    auto result = processToString(router, *Requests::PostRequest);
    testFirstAndSecondFunction(result);
}

TEST(Router, Should_have_rvalue_copy_and_assignment_operators)
{
    router router, router3;
    router.onPost("/path/somewhere", firstFunction, secondFunction);
    auto router2 = std::move(router);
    auto result = processToString(router2, *Requests::PostRequest);
    testFirstAndSecondFunction(result);
    router3 = std::move(router2);
    result = processToString(router3, *Requests::PostRequest);
    testFirstAndSecondFunction(result);
}
