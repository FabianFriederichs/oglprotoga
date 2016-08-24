#pragma once
#include "Transform.h"
class Transformable
{
public:
	Transform& getTransform() { return m_transform; }
protected:
	Transform m_transform;
};