#pragma once
#include "Light.h"
#include "glerror.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 _direction, Transform _transform, glm::vec4 _color);
	~DirectionalLight();

	glm::vec3 m_direction;
};

