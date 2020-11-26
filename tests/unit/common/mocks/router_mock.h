/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#pragma once
#include <gmock/gmock.h>
#include "request/request.h"

class RouterMock
{
public:
    static inline std::string ExpectedFakeResult = "Some Router Result...";
    
    MOCK_CONST_METHOD1(process, std::string(const cpphttp::request::request &));

    void createFakeProcess();
};
