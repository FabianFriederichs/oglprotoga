#include "Camera.h"
#pragma once
class THRDPCamera : public Camera
{
public:
	THRDPCamera(void);
	~THRDPCamera(void);
	glm::mat4 Orientation() const override;
	glm::mat4 GetViewMatrix() const override;
	void SetPosition(vec3 pos) override;
	void Move(const MoveData &movedata) override;
	void Rotate(const vec3 &rotation) override;
};