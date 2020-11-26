/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "router_mock.h"

void RouterMock::createFakeProcess()
{
    ON_CALL(*this, process).WillByDefault(testing::Return(RouterMock::ExpectedFakeResult));
}
