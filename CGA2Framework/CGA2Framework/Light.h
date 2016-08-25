#pragma once
#include "headers.h"
#include "Transform.h"
#include "glerror.h"
#include "Transformable.h"
class Light : public Transformable
{
public:
	Light();
	Light(const Light& _other);
	Light(glm::vec4 _lightcolor, Transform _transform);
	~Light();

	glm::vec4 m_lightcolor;

};

