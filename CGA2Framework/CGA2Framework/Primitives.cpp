#include "Primitives.h"

static const GLfloat CubeVerts[] = {
	// Positions          
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};
const size_t Primitives::SizeOfCubeVertices = sizeof(CubeVerts);

const GLfloat *Primitives::CubeVertices = CubeVerts;
const std::vector<Vertex> Primitives::CubeVerticesV = {};
const std::vector<GLuint> Primitives::CubeIndices = {};
static const GLfloat QuadVerts[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// Positions   // TexCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};
const size_t Primitives::SizeOfQuadVertices = sizeof(QuadVerts);
const GLfloat *Primitives::QuadVertices = QuadVerts;
const std::vector<Vertex> Primitives::QuadVerticesV = { Vertex(glm::vec3(-1.f, 1.f, 0.f), glm::vec2(0.f, 1.f)), Vertex(glm::vec3(-1.f, -1.f, 0.f), glm::vec2(0.f, 0.f)), Vertex(glm::vec3(1.f, -1.f, 0.f), glm::vec2(1.f, 0.f)), Vertex(glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f)) };
const std::vector<GLuint> Primitives::QuadIndices = { 0, 1, 2, 0, 2, 3 };

//initialize gl handles
GLuint Primitives::cubevbo = 0;
GLuint Primitives::cubevao = 0;
GLuint Primitives::quadvbo = 0;
GLuint Primitives::quadvao = 0;

void Primitives::drawNDCCube()
{
	if (cubevao == 0)
	{
		glGenVertexArrays(1, &cubevao);
		if (checkglerror())
		{
			cubevao = 0;
			return;
		}
		if (cubevbo != 0)
		{
			glDeleteBuffers(1, &cubevbo); GLERR
			cubevbo = 0;
		}
		glGenBuffers(1, &cubevbo);
		if (checkglerror())
		{
			cubevbo = 0;
			return;
		}
		glBindVertexArray(cubevao); GLERR
		glBindBuffer(GL_ARRAY_BUFFER, cubevbo); GLERR
		glBufferData(GL_ARRAY_BUFFER, SizeOfCubeVertices, CubeVertices, GL_STATIC_DRAW); GLERR
		glEnableVertexAttribArray(0); GLERR
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, reinterpret_cast<void*>(0));
		glBindVertexArray(0);		
	}

	glBindVertexArray(cubevao); GLERR
	glDrawArrays(GL_TRIANGLES, 0, 36); GLERR
	glBindVertexArray(0); GLERR
}

void Primitives::drawNDCQuad()
{
	if (quadvao == 0)
	{
		glGenVertexArrays(1, &quadvao);
		if (checkglerror())
		{
			quadvao = 0;
			return;
		}
		if (quadvbo != 0)
		{
			glDeleteBuffers(1, &quadvbo); GLERR
				quadvbo = 0;
		}
		glGenBuffers(1, &quadvbo);
		if (checkglerror())
		{
			quadvbo = 0;
			return;
		}
		glBindVertexArray(quadvao); GLERR
		glBindBuffer(GL_ARRAY_BUFFER, quadvbo); GLERR
		glBufferData(GL_ARRAY_BUFFER, SizeOfQuadVertices, QuadVertices, GL_STATIC_DRAW); GLERR
		glEnableVertexAttribArray(0); GLERR
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, reinterpret_cast<void*>(2 * sizeof(GLfloat)));
		glBindVertexArray(0);
	}

	glBindVertexArray(quadvao); GLERR
	glDrawArrays(GL_TRIANGLES, 0, 6); GLERR
	glBindVertexArray(0); GLERR
}

std::vector<Vertex> Primitives::getVertices(PRIMITIVETYPE _type)
{
	switch (_type)
	{
	case PRIMITIVETYPE::QUAD:
		return QuadVerticesV;
		break;
	case PRIMITIVETYPE::CUBE:
		return CubeVerticesV;
		break;
	default:
		return QuadVerticesV;
		break;
	}
}

std::vector<GLuint> Primitives::getIndices(PRIMITIVETYPE _type)
{
	switch (_type)
	{
	case PRIMITIVETYPE::QUAD:
		return QuadIndices;
		break;
	case PRIMITIVETYPE::CUBE:
		return CubeIndices;
		break;
	default:
		return QuadIndices;
		break;
	}
}