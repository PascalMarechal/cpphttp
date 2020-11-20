#include <gtest/gtest.h>
#include "internal/tools/params.h"
#include "common/requests.h"

using namespace cpphttp;
using namespace tools;

TEST(Params, Should_extract_url_variables)
{
    request::request req;
    req.setHeader(Requests::GetRequestHeaderWithParam);
    extractParameters("/item/:id", req);

    EXPECT_EQ(req.get("id"), "13");
}

TEST(Params, Should_extract_all_type_of_url_variables)
{
    request::request req;
    req.setHeader(Requests::GetRequestHeaderWithParam2);
    extractParameters("/complex/:what/:text/url", req);
    extractTailingParameters(req);
    
    EXPECT_EQ(req.get("what"), "13");
    EXPECT_EQ(req.get("text"), "hello");
    EXPECT_EQ(req.get("id"), "24");
    EXPECT_EQ(req.get("location"), "France");
}
