#include "Light.h"


Light::Light() :
	m_transform(),
	m_lightcolor(1.0f, 1.0f, 1.0f, 1.0f)
{
}

Light::Light(const Light& _other) :
	m_transform(_other.m_transform),
	m_lightcolor(_other.m_lightcolor)
{
}

Light::Light(glm::vec4 _lightcolor, Transform _transform) :
	m_transform(_transform),
	m_lightcolor(_lightcolor)
{
}


Light::~Light()
{
}
