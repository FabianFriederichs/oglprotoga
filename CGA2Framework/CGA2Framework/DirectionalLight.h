#pragma once
#include "Light.h"
#include "glerror.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 _direction, Transform _transform, glm::vec4 _color);
	~DirectionalLight();

	glm::mat4 getProj(GLfloat near, GLfloat far)
	{
		return glm::ortho(-512.f, 512.f, -512.f, 512.f, near, far);
	}

	glm::mat4 getView(vec3 pos)
	{
		return mat4_cast(getTransform().getRotateQ())*translate(glm::mat4(1.0f), -pos);
	}

	glm::vec3 m_direction;
};

