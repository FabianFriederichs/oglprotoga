#pragma once
#include "Camera.h"
#include "glerror.h"

class FPSCamera: public Camera
{
public:
	FPSCamera(GLfloat fov,GLint width,GLint height,GLfloat znear,GLfloat zfar, vec3 worldup, vec3 worldright, vec3 worldforward);
	//~FPSCamera(void);
	glm::mat4 Orientation() override;
	glm::mat4 GetViewMatrix() override;
	void SetPosition(vec3 pos) override;
	void Move(const MoveData &movedata) override;
	void Rotate(const vec3 &rotation) override;
	void Fly(bool);
	const bool isFlying(){ return flying; }
private:
	GLfloat m_VA;
	GLfloat m_HA;
	bool flying;
};

