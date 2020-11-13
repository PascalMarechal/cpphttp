#include <gtest/gtest.h>
#include "internal/container/ts_fifo.h"

using namespace cpphttp::internal;

TEST(Fifo, Empty)
{
    ts_fifo<int> myFifo;
    EXPECT_TRUE(myFifo.isEmpty());
}

TEST(Fifo, NotEmpty)
{
    ts_fifo<int> myFifo;
    myFifo.push(17);
    EXPECT_TRUE(!myFifo.isEmpty());
}

TEST(Fifo, GetDataInOrder)
{
    ts_fifo<int> myFifo;
    myFifo.push(17);
    myFifo.push(42);
    EXPECT_EQ(myFifo.pop(), 17);
    EXPECT_EQ(myFifo.pop(), 42);
}

TEST(Fifo, PopIfEmptyShouldThrow)
{
    ts_fifo<int> myFifo;
    EXPECT_THROW(myFifo.pop(), std::logic_error);
}
