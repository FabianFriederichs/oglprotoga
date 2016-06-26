#pragma once
#include "Light.h"
class PointLight : public Light
{
public:
	PointLight();
	~PointLight();

	//attentuation
	GLfloat m_constant;
	GLfloat m_linear;
	GLfloat m_quadratic;

	GLfloat m_radius;
};

