#pragma once
#include "headers.h"
#include "Vertex.h"
#include "glerror.h"
typedef enum{
	QUAD,
	CUBE
} PRIMITIVETYPE;

class Primitives
{
public:
	static std::vector<Vertex> getVertices(PRIMITIVETYPE _type);
	static std::vector<GLuint> getIndices(PRIMITIVETYPE _type);
	static const GLfloat *CubeVertices;
	static const size_t SizeOfCubeVertices;
	static const std::vector<Vertex> CubeVerticesV;
	static const std::vector<GLuint> CubeIndices;
	static const GLfloat *QuadVertices;
	static const size_t SizeOfQuadVertices;
	static const std::vector<Vertex> QuadVerticesV;
	static const std::vector<GLuint> QuadIndices;
	static void drawNDCCube();
	static void drawNDCQuad();

private:
	static GLuint cubevbo;
	static GLuint cubevao;
	static GLuint quadvbo;
	static GLuint quadvao;
};
