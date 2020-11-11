#include "requests.h"
#include "tools/data_reader.h"

std::string postRequestHeader = readFile("./data/headers/post_request.txt");
std::string postRequestHeaderWithIncorrectLength = readFile("./data/headers/post_request_incorrect_length.txt");
std::string postRequestBody = readFile("./data/body/post_request.txt");
