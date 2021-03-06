#pragma once
#include "headers.h"
#include "Rotatable.h"
#include "Moveable.h"
#include "GameObject.h"
using namespace glm;

/*
rotate with quaternions instead to circumvent gimbal lock??

default rotate values?

Direction value has to be the negative of the direction the camera points at
-abstract that and negate in constructor?
*/




class Camera: public GameObject
{
protected:
	vec3 m_position;
	quat m_rot;
	vec3 m_wy;
	vec3 m_wx;
	vec3 m_wz;
	GLfloat m_fov;
	GLfloat m_width;
	GLfloat m_height;
	GLfloat m_znear;
	GLfloat m_zfar;
	mat4 proj;
	virtual glm::mat4 Camera::Orientation()  =0;

public:
	virtual glm::mat4 GetViewMatrix()  =0;

	virtual vec3 GetPosition()
	{
		return getTransform().getTranslate();
	}
	virtual void SetPosition(vec3 pos) = 0;
	virtual void Move(const MoveData &movedata) =0;

	virtual void Rotate(const vec3 &rotation) = 0;




	glm::mat4 Camera::GetCameraTransform()
	{
		return  Orientation()*translate(glm::mat4(1.0f), -GetPosition());
	}

	glm::mat4 getCameraTranslation()
	{
		return translate(glm::mat4(1.0f), -GetPosition());
	}

	glm::mat4 getProjectionMatrix() const
	{
		return proj;
	}

	void recalcProj()
	{
		proj = glm::perspective(m_fov, m_width / m_height, m_znear, m_zfar);
	}

	const GLfloat getFov(){ return m_fov; }
	const GLfloat getWidth(){ return m_width; }
	const GLfloat getHeight(){ return m_height; }
	const GLfloat getNear(){ return m_znear; }
	const GLfloat getFar(){ return m_zfar; }
	const vec3 getWorldUp(){ return m_wy; }
	const vec3 getWorldRight(){ return m_wx; }
	const vec3 getWorldForw(){ return m_wz; }
};

