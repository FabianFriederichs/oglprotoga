#include "GameObject.h"
#include "IDProvider.h"


GameObject::GameObject() :
	m_active(true),
	m_name(),
	m_id(IDProvider::getInstance().createID()),
	m_transform()
{
}

GameObject::GameObject(const GameObject& _other) :
	m_active(_other.m_active),
	m_name(_other.m_name),
	m_id(IDProvider::getInstance().createID()),
	m_transform(_other.m_transform)
{
	
}


GameObject::~GameObject()
{
	
}

