#pragma once
#include "headers.h"
#include "Transform.h"
class Light
{
public:
	Light();
	Light(const Light& _other);
	Light(glm::vec4 _lightcolor, Transform _transform);
	~Light();

	//getters / setters
	Transform& getTransform() { return m_transform; }
	void setTransform(Transform& _transform) { m_transform = _transform; }

	glm::vec4 m_lightcolor;

protected:
	Transform m_transform;
};

