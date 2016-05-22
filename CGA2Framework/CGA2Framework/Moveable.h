#pragma once
#include "headers.h"
using namespace glm;
struct MoveData
{
	
	GLfloat Right;
	GLfloat Up;
	GLfloat Forward;
	GLfloat Speed;

	glm::vec4 asVec4() const
	{
		return vec4(Right,Up,Forward,Speed);
	}

	glm::vec3 asVec3() const
	{
		return vec3(Right,Up,Forward);
	}
};

class Moveable
{
public:
	virtual void Move(const MoveData &data) = 0;
};