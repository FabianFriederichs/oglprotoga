#pragma once
#include "headers.h"
#include "Transform.h"
class Light
{
public:
	Light();
	Light(const Light& _other);
	Light(const glm::vec4 _lightcolor, const glm::vec4 _fadeoutcolor);
	~Light();

	//getters / setters
	Transform& getTransform() { return m_transform; }
	void setTransform(const Transform& _transform) { m_transform = _transform; }

	glm::vec4 m_lightcolor;
	glm::vec4 m_fadeoutcolor;

private:
	Transform m_transform;
};

