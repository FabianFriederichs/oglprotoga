#pragma once
#include "Scene.h"
#include "FPSCamera.h"
#include "glerror.h"
class TestScene :
	public Scene
{
public:
	TestScene(GLint width, GLint height);
	~TestScene();

	void save(const std::string&);
	void save(){};
	void load();
	void load(const std::string&);
};

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