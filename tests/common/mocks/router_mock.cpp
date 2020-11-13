#include "router_mock.h"

void RouterMock::createFakeProcess()
{
    ON_CALL(*this, process).WillByDefault(testing::Return(RouterMock::ExpectedFakeResult));
}
