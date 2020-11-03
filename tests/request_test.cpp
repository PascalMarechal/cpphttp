#include "request.h"
#include <gtest/gtest.h>

using namespace cpphttp;

TEST(Request, EmptyRequest)
{
    request req;
    EXPECT_EQ(req.isReady(), false);
}
