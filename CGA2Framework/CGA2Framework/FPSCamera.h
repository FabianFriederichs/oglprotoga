#include "Camera.h"
#pragma once
class FPSCamera: public Camera
{
public:
	FPSCamera(GLfloat fov,GLfloat width,GLfloat height,GLfloat znear,GLfloat zfar);
	~FPSCamera(void);
	glm::mat4 GetViewMatrix()const;
	void Move(const MoveData &movedata);
	void Rotate(const quat &orientation);
private:
	GLfloat m_VA;
	GLfloat m_HA;
};

