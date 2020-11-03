#include "data_reader.h"
#include <fstream>
#include <streambuf>

std::string readFile(const std::string &path)
{
    std::ifstream t(path);
    return std::string((std::istreambuf_iterator<char>(t)),
                       std::istreambuf_iterator<char>());
}
