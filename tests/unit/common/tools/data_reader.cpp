/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "data_reader.h"
#include <fstream>
#include <streambuf>

std::string readFile(const std::string &path)
{
    std::ifstream t(path);
    return std::string((std::istreambuf_iterator<char>(t)),
                       std::istreambuf_iterator<char>());
}
