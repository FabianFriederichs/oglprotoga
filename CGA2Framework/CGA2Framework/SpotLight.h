#pragma once
#include "DirectionalLight.h"
class SpotLight : public Light
{
public:
	SpotLight();
	SpotLight(glm::vec4 _color, Transform _transform, glm::vec3 _direction, GLfloat _constant, GLfloat _linear, GLfloat _quadratic, GLfloat _range);
	~SpotLight();

	glm::vec3 m_direction;
	GLfloat m_cosinnercone;
	GLfloat m_cosoutercone;

	//attentuation
	GLfloat m_constant;
	GLfloat m_linear;
	GLfloat m_quadratic;

	GLfloat m_range;
};

