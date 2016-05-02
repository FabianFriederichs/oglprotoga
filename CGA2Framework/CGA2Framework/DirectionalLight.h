#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	glm::vec3 m_direction;
};

