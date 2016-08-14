#pragma once
#include "headers.h"


//string trim
void rtrimstr(std::string& _string);
void ltrimstr(std::string& _string);
void trimstr(std::string& _string);

std::vector<int> extractInts(const std::string& _s, char _d);

inline GLint max(const GLint a, const GLint b) { return (a > b ? a : b); }
inline GLint min(const GLint a, const GLint b) { return (a < b ? a : b); }