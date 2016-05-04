#pragma once
#include "DirectionalLight.h"
class SpotLight : public DirectionalLight
{
public:
	SpotLight();
	~SpotLight();

	void setOuterConeAngle(GLfloat _angle, bool _radians);
	void setInnerCodeAngle(GLfloat _angle, bool _radians);

	GLfloat m_cosinnercone;
	GLfloat m_cosoutercone;

	//attentuation
	GLfloat m_linear;
	GLfloat m_quadratic;
	GLfloat m_cubic;
};

