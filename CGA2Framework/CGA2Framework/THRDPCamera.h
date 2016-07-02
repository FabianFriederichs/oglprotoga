#include "Camera.h"
#pragma once
class THRDPCamera : public Camera
{
public:
	THRDPCamera(void);
	~THRDPCamera(void);
	glm::mat4 GetViewMatrix()const;
	void Move(const MoveData &movedata);
	void Rotate(const quat &orientation);
};