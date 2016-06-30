#include "Camera.h"
#pragma once
class FPSCamera: public Camera
{
public:
	FPSCamera(void);
	~FPSCamera(void);
	glm::mat4 GetViewMatrix()const;
	void Move(const MoveData &movedata);
	void Rotate(const quat &orientation);
};

