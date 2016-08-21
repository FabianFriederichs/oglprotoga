#pragma once
#include "headers.h"


//string trim
void rtrimstr(std::string& _string);
void ltrimstr(std::string& _string);
void trimstr(std::string& _string);

std::vector<int> extractInts(const std::string& _s, char _d);

inline GLint max(const GLint a, const GLint b) { return (a > b ? a : b); }
inline GLint min(const GLint a, const GLint b) { return (a < b ? a : b); }

void split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

const std::string vec3ToString(const glm::vec3& v);

const std::string vec4ToString(const glm::vec4& v);

const glm::vec3 vec3FromString(const std::string& v);

const glm::vec4 vec4FromString(const std::string& v);

const std::string Peek(std::ifstream &strim);

class SNode
{
public:
	~SNode()
	{
	}
	SNode(){ parent = nullptr; }
	SNode(std::string value){ m_value = value; parent = nullptr; }
	const std::string Value() { return m_value; }
	SNode& operator[](int idx) const
	{

		if (idx < 0)
		{
			idx = abs(idx);
			if (--idx >= children.size())
				throw "Index out of range!";
			auto it = children.rbegin();
			std::advance(it, idx);
			return **it;
		}
		else
		{
			if (idx >= children.size())
				throw "Index out of range!";
			auto it = std::begin(children);
			std::advance(it, idx);
			return **it;
		}
	}
	SNode& operator[](std::string val) const
	{
		for (SNode* sn : children)
		{
			if (sn->Value() == val)
				return *sn;
		}
		throw "Element doesn't exist!";
	}

	bool AddChild(SNode* node)
	{
		auto s = node->Value();
		if (!Contains(s))
		{
			node->setParent(this);
			children.push_back(node);
			return true;
		}
		return false;
	}
	bool AddChild(std::string &val)
	{
		if (!Contains(val))
		{
			children.push_back(new SNode(val));
			children.back()->setParent(this);
			return true;
		}
		return false;
	}

	void setParent(SNode *p)
	{
		parent = p;
	}

	const bool Contains(const std::string &val)
	{
		for (SNode* sn : children)
		{
			if (sn->Value() == val)
				return true;
		}
		return false;
	}

	SNode& getNode(std::string val) const
	{
		return this->operator[](val);
	}

	SNode& getNode(int val) const
	{
		return this->operator[](val);
	}

	std::size_t Count() const { return children.size(); }

	std::list<SNode*> Children() const{ return children; }

private:
	std::list<SNode*> children;
	SNode *parent;
	std::string m_value;
};

glm::mat4 convert(const vr::HmdMatrix44_t &m);

glm::mat4x3 convert(const vr::HmdMatrix34_t &m);

void resolveFB(GLint read, GLint draw, glm::uint32 renderwidth, glm::uint32 renderheight);