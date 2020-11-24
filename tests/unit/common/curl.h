#pragma once

#include <string>

bool checkUrl(const char *url);
std::string getPage(const char *url);
std::string postAndThenGetAnotherPage(const char *postUrl, const char *postData, const char *getUrl);
