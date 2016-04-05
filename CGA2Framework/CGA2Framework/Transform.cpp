#include "Transform.h"
//TODO:	Implement quaternions and think of returning references instead of copies

Transform::Transform(const glm::vec3& _translate, const glm::vec3& _rotate, const glm::vec3& _scale) :
	m_translate(_translate),
	m_rotate(_rotate),
	m_scale(_scale),
	m_transformMat(1.0f),
	m_matdirty(false)
{
	
}

Transform::Transform(const Transform& _other) :
	m_translate(_other.m_translate),
	m_rotate(_other.m_rotate),
	m_scale(_other.m_scale),
	m_transformMat(_other.m_transformMat),
	m_matdirty(true)
{
	
}

Transform::Transform() :
	m_translate(0.0f, 0.0f, 0.0f),
	m_rotate(0.0f, 0.0f, 0.0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_transformMat(1.0f),
	m_matdirty(false)
{
	
}


Transform::~Transform()
{
}

void Transform::translate(const glm::vec3& _translate)
{
	m_translate += _translate;
	m_matdirty = true;
}

void Transform::rotate(const glm::vec3& _rotate)
{
	m_rotate += _rotate;
	m_matdirty = true;
}

void Transform::rotate(const GLfloat _pitch, const GLfloat _yaw, const GLfloat _roll)
{
	m_rotate += glm::vec3(_pitch, _yaw, _roll);
	m_matdirty = true;
}

void Transform::scale(const glm::vec3& _scale)
{	
	m_scale.x *= _scale.x;
	m_scale.y *= _scale.y;
	m_scale.z *= _scale.z;
	m_matdirty = true;
}

glm::mat4 Transform::getTransformMat()
{
	if (m_matdirty)
		updateTransformMat();
	return m_transformMat;
}

void Transform::updateTransformMat()
{
	m_transformMat = glm::mat4(1.0f);
	m_transformMat = glm::translate(m_transformMat, m_translate);
	m_transformMat = glm::rotate(m_transformMat, m_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(m_transformMat, m_rotate.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(m_transformMat, m_rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
	m_transformMat = glm::scale(m_transformMat, m_scale);
	m_matdirty = false;
}
