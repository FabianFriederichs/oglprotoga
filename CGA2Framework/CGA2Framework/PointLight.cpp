#include "PointLight.h"


PointLight::PointLight() :
Light(),
m_constant(),
m_linear(),
m_quadratic(),
m_range()
{
}

PointLight::PointLight(glm::vec4 _color, Transform _transform, GLfloat _constant, GLfloat _linear, GLfloat _quadratic, GLfloat _range) :
Light(_color, _transform),
m_constant(_constant),
m_linear(_linear),
m_quadratic(_quadratic),
m_range(_range)
{

}


PointLight::~PointLight()
{
}
