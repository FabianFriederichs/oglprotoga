#pragma once
#include "headers.h"
class Vertex
{
public:
	Vertex();
	Vertex(const Vertex& _other);
	Vertex(const glm::vec3& _position,
		const glm::vec2& _uv = glm::vec2(0.0f, 0.0f),
		const glm::vec3& _normal = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3& _tangent = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3& _bitangent = glm::vec3(0.0f, 0.0f, 0.0f));
	~Vertex();

	//declare equalityoperator as friend for access to private members
	friend inline bool operator==(const Vertex& _v1, const Vertex& _v2);

	//getters / setters
	const glm::vec3& getPosition() const { return m_position; }
	const glm::vec3& getNormal() const { return m_normal; }
	const glm::vec2& getUV() const { return m_uv; }
	const glm::vec3& getTangent() const { return m_tangent; }
	const glm::vec3& getBitangent() const { return m_bitangent; }

	void setPosition(const glm::vec3& _position) { m_position = _position; }
	void setNormal(const glm::vec3& _normal) { m_normal = _normal; }
	void setUV(const glm::vec2& _uv) { m_uv = _uv; }
	void setTangent(const glm::vec3& _tangent) { m_tangent = _tangent; }
	void setBitangent(const glm::vec3& _bitangent) { m_bitangent = _bitangent; }

private:
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_uv;
	glm::vec3 m_tangent;
	glm::vec3 m_bitangent; //no need for bitangent in shader later, bitangent equals normal cross tangent!
};

//global equality operator
inline bool operator==(const Vertex& _v1, const Vertex& _v2)
{
	if (glm::all(glm::equal(_v1.m_position, _v2.m_position)) &&
		glm::all(glm::equal(_v1.m_uv, _v2.m_uv)) &&
		glm::all(glm::equal(_v1.m_normal, _v2.m_normal)) &&
		glm::all(glm::equal(_v1.m_tangent, _v2.m_tangent)) &&
		glm::all(glm::equal(_v1.m_bitangent, _v2.m_bitangent)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

