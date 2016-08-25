#pragma once
#include "Light.h"
class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec4 _color, Transform _transform, GLfloat _constant, GLfloat _linear, GLfloat _quadratic, GLfloat _range);
	~PointLight();

	GLfloat getFov(){
		return 90.f;
	}

	GLfloat getNear()
	{
		return 0.01f;
	}

	GLfloat getFar()
	{
		return m_range;
	}

	glm::mat4 GetView(glm::vec3 _face)
	{
		auto defNorm = glm::vec3(0, 0, 1);

		return  mat4_cast(getTransform().getRotateQ())*translate(glm::mat4(1.0f), -m_transform.getTranslate());
	}

	//attentuation
	GLfloat m_constant;
	GLfloat m_linear;
	GLfloat m_quadratic;

	GLfloat m_range;
};

