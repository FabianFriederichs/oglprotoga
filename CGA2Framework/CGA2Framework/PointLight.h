#pragma once
#include "Light.h"
class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec4 _color, Transform _transform, GLfloat _constant, GLfloat _linear, GLfloat _quadratic, GLfloat _range);
	~PointLight();

	//attentuation
	GLfloat m_constant;
	GLfloat m_linear;
	GLfloat m_quadratic;

	GLfloat m_range;
};

