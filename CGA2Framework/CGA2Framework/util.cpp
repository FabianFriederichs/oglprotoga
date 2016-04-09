#include "util.h"
void rtrimstr(std::string& _string)
{
	_string.erase(std::find_if(_string.rbegin(), _string.rend(), [](int x){return !std::isspace(x); }).base(), _string.end());
}

void ltrimstr(std::string& _string)
{
	_string.erase(_string.begin(), std::find_if(_string.begin(), _string.end(), [](int x){return !std::isspace(x); }));
}

void trimstr(std::string& _string)
{
	ltrimstr(_string);
	rtrimstr(_string);
}