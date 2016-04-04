#pragma once
#include "headers.h"
class Transform
{
public:
	Transform(const glm::vec3& _translate, const glm::vec3& _rotate, const glm::vec3& _scale);
	Transform();
	Transform(const Transform& _other);
	~Transform();

	void translate(const glm::vec3& _translate);
	void rotate(const glm::vec3& _rotate);
	void rotate(const GLfloat _pitch, const GLfloat _yaw, const GLfloat _roll);
	void scale(const glm::vec3& _scale);
	glm::mat4 getTransformMat();

	//getters, setters
	const glm::vec3& getTranslate()  const { return m_translate; }
	const glm::vec3& getRotate()  const { return m_rotate; }
	const glm::vec3& getScale()  const { return m_scale; }

	void setTranslate(const glm::vec3& _translate) { m_translate = _translate; m_matdirty = true; }
	void setRotate(const glm::vec3& _rotate) { m_rotate = _rotate; m_matdirty = true; }
	void setRotate(const GLfloat _pitch, const GLfloat _yaw, const GLfloat _roll) { m_rotate = glm::vec3(_pitch, _yaw, _roll); m_matdirty = true; }
	void setScale(const glm::vec3& _scale) { m_scale = _scale; m_matdirty = true; }

private:
	glm::vec3 m_translate;
	glm::vec3 m_rotate;
	glm::vec3 m_scale;

	bool m_matdirty;
	glm::mat4 m_transformMat;
	void updateTransformMat();

};

