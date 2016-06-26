#pragma once
#include "DirectionalLight.h"
class SpotLight : public Light
{
public:
	SpotLight();
	~SpotLight();

	void setOuterConeAngle(GLfloat _angle, bool _radians);
	void setInnerCodeAngle(GLfloat _angle, bool _radians);

	glm::vec3 m_direction;
	GLfloat m_cosinnercone;
	GLfloat m_cosoutercone;

	//attentuation
	GLfloat m_constant;
	GLfloat m_linear;
	GLfloat m_quadratic;
};

