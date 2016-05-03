#pragma once
#include "DirectionalLight.h"
class SpotLight : public DirectionalLight
{
public:
	SpotLight();
	~SpotLight();

	GLfloat m_cosinnercone;
	GLfloat m_cosoutercone;

	//attentuation
	GLfloat m_linear;
	GLfloat m_quadratic;
	GLfloat m_cubic;
};

