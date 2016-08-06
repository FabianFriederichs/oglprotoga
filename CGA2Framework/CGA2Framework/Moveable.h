#pragma once
#include "headers.h"
using namespace glm;

struct MoveData
{
	vec3 mtype;
	GLfloat Multiplier
};

class Moveable
{
public:
	virtual void Move(const MoveData &data) = 0;
};