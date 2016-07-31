#include "Camera.h"
#pragma once
class FPSCamera: public Camera
{
public:
	FPSCamera(GLfloat fov,GLint width,GLint height,GLfloat znear,GLfloat zfar, vec3 up);
	//~FPSCamera(void);
	glm::mat4 Orientation() const override;
	glm::mat4 GetViewMatrix() const override;
	void Move(const MoveData &movedata) override;
	void Rotate(const vec3 &rotation) override;
private:
	GLfloat m_VA;
	GLfloat m_HA;
};

