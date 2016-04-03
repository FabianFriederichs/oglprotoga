#pragma once
#include "headers.h"
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