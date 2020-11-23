#include "request/request.h"
#include <gtest/gtest.h>
#include "common/requests.h"

using namespace cpphttp::request;

TEST(Request, Complete_request_with_no_body)
{
    request req;
    req.setHeader(Requests::GetRequestHeader);
    EXPECT_TRUE(req.isReady());
}

TEST(Request, Complete_request_with_missing_body)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    EXPECT_FALSE(req.isReady());
}

TEST(Request, Complete_request_with_incomplete_body)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    req.setBody("too short");
    EXPECT_FALSE(req.isReady());
}

TEST(Request, Complete_request_with_complete_body)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    req.setBody(Requests::PostRequestBody);
    EXPECT_TRUE(req.isReady());
}

TEST(Request, Get_header)
{
    request req;
    EXPECT_THROW(req.header(), std::invalid_argument);

    req.setHeader("Something");
    EXPECT_NO_THROW(req.header());
}

TEST(Request, Equality_operators)
{
    request req;
    req.setHeader(Requests::PostRequestHeader);
    req.setBody(Requests::PostRequestBody);
    request req2;
    req2.setHeader(Requests::PostRequestHeader);
    req2.setBody(Requests::PostRequestBody);
    EXPECT_TRUE(req == req2);

    request req3, req4;
    EXPECT_TRUE(req3 == req4);
    EXPECT_TRUE(req != req3);

    req3.setHeader(Requests::PostRequestHeader);
    EXPECT_TRUE(req3 != req);

    req3.setBody("nooope");
    EXPECT_TRUE(req3 != req);

    req4.setBody(Requests::PostRequestBody);
    EXPECT_TRUE(req4 != req);

    req2.setHeader("noope");
    EXPECT_TRUE(req != req2);
}

TEST(Request, Set_temp_variables)
{
    request req;
    req.set("name", "value");
    req.set("name2", "value2");
    req.set("int", 12);
    req.set("float", 12.54f);

    EXPECT_EQ(req.get("name"), "value");
    EXPECT_EQ(req.get("name2"), "value2");
    EXPECT_EQ(req.get("nothere"), "");
    EXPECT_EQ(req.get("int"), "12");
    EXPECT_EQ(req.get("float"), "12.540000");

    EXPECT_TRUE(req.has("name"));
    EXPECT_TRUE(req.has("name2"));
    EXPECT_FALSE(req.has("nothere"));
    EXPECT_TRUE(req.has("int"));
    EXPECT_TRUE(req.has("float"));
}

TEST(Request, Get_request_should_have_tailing_params_loaded)
{
    EXPECT_EQ(Requests::GetRequestWithTailingParams->getParam("id"), "24");
    EXPECT_EQ(Requests::GetRequestWithTailingParams->getParam("location"), "France");
}

TEST(Request, Request_should_load_url_param_according_to_expected_path)
{
    request req;
    req.setHeader(Requests::GetRequestHeaderWithParam);
    req.loadParamFromUrl("/item/:index");

    EXPECT_EQ(req.getParam("index"), "13");

    request req2;
    req2.setHeader(Requests::GetRequestHeaderWithParam2);
    req2.loadParamFromUrl("/complex/:val/:text/url");
    EXPECT_EQ(req2.getParam("val"), "13");
    EXPECT_EQ(req2.getParam("text"), "hello");
}

TEST(Request, should_post_data_from_form_urlencoded_body)
{
    EXPECT_EQ(Requests::PostRequest->getParam("home"), "Cosby");
    EXPECT_EQ(Requests::PostRequest->getParam("favorite+flavor"), "flies");
}
