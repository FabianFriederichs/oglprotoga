#include "Transform.h"
//TODO:	Implement quaternions and think of returning references instead of copies

Transform::Transform(const glm::vec3& _translate, const glm::vec3& _rotate, const glm::vec3& _scale) :
	m_translate(_translate),
	m_rquat(_rotate),
	m_scale(_scale),
	m_transformMat(1.0f),
	m_matdirty(true),
	m_matoverride(false),
	m_stayOverriden(false)
{
	
}

Transform::Transform(const Transform& _other) :
	m_translate(_other.m_translate),
	m_rquat(_other.m_rotate),
	m_scale(_other.m_scale),
	m_transformMat(_other.m_transformMat),
	m_matdirty(true),
	m_matoverride(false),
	m_stayOverriden(false)
{
	
}

Transform::Transform() :
	m_translate(0.0f, 0.0f, 0.0f),
	m_rquat(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_scale(1.0f, 1.0f, 1.0f),
	m_transformMat(1.0f),
	m_matdirty(true),
	m_matoverride(false),
	m_stayOverriden(false)
{
	
}


Transform::~Transform()
{
}

void Transform::translate(const glm::vec3& _translate)
{
	m_translate += _translate;
	if (!m_stayOverriden)
		m_matdirty = true;
}

void Transform::rotate(const glm::vec3& _rotate)
{
	m_rquat = normalize(m_rquat*glm::quat(_rotate));
	
	if (!m_stayOverriden)
		m_matdirty = true;
}

void Transform::rotate(const GLfloat _pitch, const GLfloat _yaw, const GLfloat _roll)
{
	m_rquat = normalize(m_rquat*glm::quat(glm::vec3(_pitch, _yaw, _roll)));
	
	if (!m_stayOverriden)
		m_matdirty = true;
}

void Transform::scale(const glm::vec3& _scale)
{	
	m_scale.x *= _scale.x;
	m_scale.y *= _scale.y;
	m_scale.z *= _scale.z;
	if (!m_stayOverriden)
		m_matdirty = true;
}

glm::mat4 Transform::getTransformMat()
{
	if (m_matdirty && !m_stayOverriden)
		updateTransformMat();
	return m_transformMat;
}

void Transform::updateTransformMat()
{
	m_transformMat = glm::translate(m_translate)*glm::mat4(m_rquat)*glm::scale(m_scale)* glm::mat4(1.0f);
	updateDirections();
	//m_transformMat = glm::translate(m_transformMat, m_translate);
	//
	///*m_transformMat = glm::rotate(m_transformMat, m_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
	//	glm::rotate(m_transformMat, m_rotate.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
	//	glm::rotate(m_transformMat, m_rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));*/
	//m_transformMat = glm::scale(m_transformMat, m_scale);
	m_matdirty = false;
	m_matoverride = false;
}

void Transform::updateDirections()
{
	m_z = normalize(glm::vec3(m_transformMat[0][2], m_transformMat[1][2], m_transformMat[2][2]));
	m_x = normalize(glm::vec3(m_transformMat[0][0], m_transformMat[1][0], m_transformMat[2][0]));
	m_y = normalize(glm::vec3(m_transformMat[0][1], m_transformMat[1][1], m_transformMat[2][1]));
}