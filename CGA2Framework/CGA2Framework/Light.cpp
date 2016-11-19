#include "Light.h"


Light::Light() :
	m_lightcolor(1.0f, 1.0f, 1.0f, 1.0f)
{
}

Light::Light(const Light& _other) :
	m_lightcolor(_other.m_lightcolor)
{
	setTransform(_other.m_transform);
}

Light::Light(glm::vec4 _lightcolor, Transform _transform) :
	m_lightcolor(_lightcolor)
{
	setTransform(_transform);
}


Light::~Light()
{
}
