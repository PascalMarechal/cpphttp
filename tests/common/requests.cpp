#include "requests.h"
#include "common/tools/data_reader.h"

std::string Requests::PostRequestHeader = readFile("./data/headers/post_request.txt");
std::string Requests::GetRequestHeader = readFile("./data/headers/get_request.txt");
std::string Requests::PostRequestHeaderWithIncorrectLength = readFile("./data/headers/post_request_incorrect_length.txt");
std::string Requests::PostRequestBody = readFile("./data/body/post_request.txt");
