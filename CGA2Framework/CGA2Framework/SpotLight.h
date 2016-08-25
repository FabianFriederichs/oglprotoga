#pragma once
#include "DirectionalLight.h"
class SpotLight : public Light
{
public:
	SpotLight();
	SpotLight(glm::vec4 _color, Transform _transform, glm::vec3 _direction, GLfloat _constant, GLfloat _linear, GLfloat _quadratic, GLfloat _range);
	~SpotLight();

	GLfloat getFov(){
		return acos(m_cosoutercone) * 2;
	}

	GLfloat getNear()
	{
		return 0.01f;
	}

	GLfloat getFar()
	{
		return m_range;
	}

	glm::mat4 GetView()
	{
		return  mat4_cast(getTransform().getRotateQ())*translate(glm::mat4(1.0f), -m_transform.getTranslate());
	}

	GLfloat m_cosinnercone;
	GLfloat m_cosoutercone;

	//attentuation
	GLfloat m_constant;
	GLfloat m_linear;
	GLfloat m_quadratic;

	GLfloat m_range;
};

