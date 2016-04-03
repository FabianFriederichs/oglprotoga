#pragma once
#include "headers.h"
struct MoveData
{
	GLfloat Speed;
	GLfloat Forward;
	GLfloat Backward;
	GLfloat Left;
	GLfloat Right;
};

class Moveable
{
public:
	virtual void Move(const MoveData &data) = 0;
};