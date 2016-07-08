#include "DirectionalLight.h"


DirectionalLight::DirectionalLight() :
	Light(),
	m_direction(glm::vec3(0.0f, 0.0f, -1.0f))
{
}

DirectionalLight::DirectionalLight(glm::vec3 _direction, Transform _transform, glm::vec4 _color) :
	Light(_color, _transform),
	m_direction(_direction)
{

}



DirectionalLight::~DirectionalLight()
{
}
