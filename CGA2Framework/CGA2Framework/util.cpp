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
	for (unsigned int i = 0; i < buf.size(); i++)
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

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

const std::string vec3ToString(const glm::vec3& v)
{
	std::string s = "";
	s += std::to_string(v.x) + ",";
	s += std::to_string(v.y) + ",";
	s += std::to_string(v.z);
	return s;
}

const std::string vec4ToString(const glm::vec4& v)
{
	std::string s = "";
	s += std::to_string(v.x) + ",";
	s += std::to_string(v.y) + ",";
	s += std::to_string(v.z) + ",";
	s += std::to_string(v.w);
	return s;
}


const glm::vec3 vec3FromString(const std::string& v)
{

	auto vec = split(v, ',');
	GLfloat x(std::stod(vec[0])), y(std::stod(vec[1])), z(std::stod(vec[2]));
	return glm::vec3(x, y, z);
}

const glm::vec4 vec4FromString(const std::string& v)
{
	auto vec = split(v, ',');
	GLfloat x(std::stod(vec[0])), y(std::stod(vec[1])), z(std::stod(vec[2])), w(std::stod(vec[3]));

	return glm::vec4(x, y, z, w);
}

const std::string Peek(std::ifstream &strim)
{
	std::string line;
	int len = strim.tellg();
	// Read line
	std::getline(strim, line);
	// Return to position before "Read line".
	strim.seekg(len, std::ios_base::beg);
	return line;
}
