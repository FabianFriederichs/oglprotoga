#pragma once
#include "DirectionalLight.h"
class SpotLight : public DirectionalLight
{
public:
	SpotLight();
	~SpotLight();

	GLfloat m_cosinnercone;
	GLfloat m_cosoutercone;
};

