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

std::vector<int> extractInts(const std::string& _s, char _d)
{
	std::stringstream strm(_s);
	std::vector<int> result;
	std::vector<std::string> buf;
	std::string tmp;
	int itmp;
	while (std::getline(strm, tmp, _d))
	{
		trimstr(tmp);
		buf.push_back(tmp);
	}
	strm.clear();
	for (int i = 0; i < buf.size(); i++)
	{
		strm << buf[i] << " ";
	}
	strm.seekg(0, strm.beg);
	while (strm >> itmp)
	{
		result.push_back(itmp);
	}
	return result;
}