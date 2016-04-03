#pragma once
#include "headers.h"

/*
Add functionality for rotations via euler angles
Add functionality for rotations via quaternions

*/
struct RotateData
{
	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Roll;
};

class Rotatable
{
public:
	virtual void Rotate(const RotateData &data) = 0;
};