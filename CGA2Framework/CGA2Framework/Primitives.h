#pragma once
#include "headers.h"
#include "glerror.h"
class Primitives
{
public:
	static const GLfloat *CubeVertices;
	static const size_t SizeOfCubeVertices;

	static const GLfloat *QuadVertices;
	static const size_t SizeOfQuadVertices;

	static void drawNDCCube();
	static void drawNDCQuad();

private:
	static GLuint cubevbo;
	static GLuint cubevao;
	static GLuint quadvbo;
	static GLuint quadvao;
};
