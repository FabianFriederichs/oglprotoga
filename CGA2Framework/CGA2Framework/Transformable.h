#pragma once
#include "Transform.h"
class Transformable
{
public:
	Transformable():m_transform(){}
	Transform& getTransform() { return m_transform; }
	void setTransform(Transform _trans){ m_transform = _trans; }
protected:
	Transform m_transform;
};