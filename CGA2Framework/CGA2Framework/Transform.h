#pragma once
#include "headers.h"
class Transform
{
public:
	Transform();
	~Transform();

	void translate(glm::vec3 _translate);
	void rotate(glm::vec3 _rotate);
	void rotate(GLfloat _yaw, GLfloat _pitch, GLfloat _roll);
	void scale(glm::vec3 _scale);
	glm::mat4 getTransformMat();

private:
	glm::vec3 m_translate;
	glm::vec3 m_rotate;
	glm::vec3 m_scale;

};

