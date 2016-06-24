#pragma once
#include "headers.h"


//string trim
void rtrimstr(std::string& _string);
void ltrimstr(std::string& _string);
void trimstr(std::string& _string);

std::vector<int> extractInts(const std::string& _s, char _d);
