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
	const glm::vec3& getRotate() const { return glm::vec3(pitch(m_rquat), yaw(m_rquat), roll(m_rquat)); }
	const glm::quat& getRotateQ()  const { return m_rquat; }
	const glm::vec3& getScale()  const { return m_scale; }

	void setTranslate(const glm::vec3& _translate) { m_translate = _translate; m_matdirty = true; }
	void setRotate(const glm::vec3& _rotate) { m_rquat = glm::quat(_rotate); m_matdirty = true; }
	void setRotate(const GLfloat _pitch, const GLfloat _yaw, const GLfloat _roll) { m_rquat = glm::quat(glm::vec3(_pitch, _yaw, _roll)); m_matdirty = true; }
	void setRotate(const glm::quat& _rquat){ m_rquat = _rquat; m_matdirty = true; }
	void setScale(const glm::vec3& _scale) { m_scale = _scale; m_matdirty = true; }
	void setMatrix(const glm::mat4& _mat){ m_transformMat = _mat; updateDirections(); m_matdirty = false; m_matoverride = true; }
	const glm::vec3 getUp(){ return m_y; }
	const glm::vec3 getRight(){ return m_x; }
	const glm::vec3 getForw(){
		return m_z; }

	const bool isMatOverriden(){ return m_matoverride; }

	const bool isKeptOverriden(){ return m_stayOverriden; }
	
	void keepOverriden(bool b){ m_stayOverriden = b; }

private:
	glm::vec3 m_translate;
	glm::quat m_rquat;
	glm::vec3 m_rotate;
	glm::vec3 m_scale;
	glm::vec3 m_y;
	glm::vec3 m_x;
	glm::vec3 m_z;
	bool m_matoverride;
	bool m_matdirty;
	glm::mat4 m_transformMat;
	void updateTransformMat();
	void updateDirections();
	bool m_stayOverriden;
};

