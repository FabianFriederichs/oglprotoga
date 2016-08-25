#pragma once
#include "Transform.h"
class Transformable
{
public:
	Transformable():m_transform(){}
	Transform& getTransform() { return m_transform; }
protected:
	Transform m_transform;
};