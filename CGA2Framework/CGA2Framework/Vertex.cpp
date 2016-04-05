#include "Vertex.h"

Vertex::Vertex()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_uv = glm::vec2(0.0f, 0.0f);
	m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
	m_tangent = glm::vec3(0.0f, 0.0f, 0.0f);
	m_bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
}

Vertex::Vertex(const Vertex& _other)
{
	m_position = _other.m_position;
	m_uv = _other.m_uv;
	m_normal = _other.m_normal;
	m_tangent = _other.m_tangent;
	m_bitangent = _other.m_bitangent;
}

Vertex::Vertex(const glm::vec3& _position,
	const glm::vec2& _uv = glm::vec2(0.0f, 0.0f),
	const glm::vec3& _normal = glm::vec3(0.0f, 0.0f, 0.0f),
	const glm::vec3& _tangent = glm::vec3(0.0f, 0.0f, 0.0f),
	const glm::vec3& _bitangent = glm::vec3(0.0f, 0.0f, 0.0f))
{
	m_position = _position;
	m_uv = _uv;
	m_normal = _normal;
	m_tangent = _tangent;
	m_bitangent = _bitangent;
}

Vertex::~Vertex()
{

}

bool Vertex::operator==(const Vertex& _other)
{
	if (glm::all(glm::equal(m_position, _other.m_position)) &&
		glm::all(glm::equal(m_uv, _other.m_uv)) &&
		glm::all(glm::equal(m_normal, _other.m_normal)) &&
		glm::all(glm::equal(m_tangent, _other.m_tangent)) &&
		glm::all(glm::equal(m_bitangent, _other.m_bitangent)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
