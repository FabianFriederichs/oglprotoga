#include "Vertex.h"

Vertex::Vertex() :
	m_position(0.0f, 0.0f, 0.0f),
	m_uv(0.0f, 0.0f),
	m_normal(0.0f, 0.0f, 0.0f),
	m_tangent(0.0f, 0.0f, 0.0f),
	m_bitangent(0.0f, 0.0f, 0.0f)

{
	
}

Vertex::Vertex(const Vertex& _other) :
	m_position(_other.m_position),
	m_uv(_other.m_uv),
	m_normal(_other.m_normal),
	m_tangent(_other.m_tangent),
	m_bitangent(_other.m_bitangent)
{
	
}

Vertex::Vertex(const glm::vec3& _position,
	const glm::vec2& _uv,
	const glm::vec3& _normal,
	const glm::vec3& _tangent,
	const glm::vec3& _bitangent) :
	m_position(_position),
	m_uv(_uv),
	m_normal(_normal),
	m_tangent(_tangent),
	m_bitangent(_bitangent)
{
	
}

Vertex::~Vertex()
{

}
