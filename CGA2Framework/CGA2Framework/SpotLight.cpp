#include "SpotLight.h"


SpotLight::SpotLight() :
Light(),
m_constant(),
m_linear(),
m_quadratic(),
m_range()
{
	//m_transform.getRotateQ()
	//m_transform.setTranslate(glm::vec3(0.0f, 0.0f, -1.0f))
}

SpotLight::~SpotLight()	
{
}

SpotLight::SpotLight(glm::vec4 _color, Transform _transform, glm::vec3 _direction, GLfloat _constant, GLfloat _linear, GLfloat _quadratic, GLfloat _range) :
Light(),
//m_direction(_direction),
m_constant(_constant),
m_linear(_linear),
m_quadratic(_quadratic),
m_range(_range)
{

}
