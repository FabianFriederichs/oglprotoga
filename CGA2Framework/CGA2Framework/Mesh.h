#pragma once
#include "headers.h"
#include "Vertex.h"
#include "Material.h"
#include "Shader.h"
#include "Transform.h"
class Mesh
{
public:
	Mesh();
	Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLint>& _indices, const Material& _material, const bool _calcBoundingBox = false);
	Mesh(const Mesh& _other);
	~Mesh();

	void generateBoundingBox();
	const bool hasBoundingBox() const { return m_hasBoundingBox; }
	const bool hasNormals() const { return m_hasNormals; }
	const bool hasTexCoords() const { return m_hasTexCoords; }
	const bool hasTangents() const { return m_hasTangents; }

	void addVertex(const Vertex& _vertex);
	void removeVertex(const Vertex& _vertex);
	void addIndex(const GLint _index);
	void removeIndex(const GLint _index);

	//getters / setters
	const GLint getID() const { return m_id; }
	const GLint getPosVBOVertices() const { return m_posvbo; }
	const GLint getUVVBOVertices() const { return m_uvvbo; }
	const GLint getNormVBOVertices() const { return m_normvbo; }
	const GLint getTanVBOVertices() const { return m_tanvbo; }
	const GLint getBitanVBOVertices() const { return m_bitanvbo; }
	const GLint getIBOIndices() const { return m_ibo; }
	const GLint getVBOBoundingBox() const { return m_vboBoundingBoxPos; }
	const GLint getIBOBoundingBox() const { return m_iboBoundingBox; }
	const GLint getVAOVertices() const { return m_vao; }
	const GLint getVAOBoundingBox() const { return m_vaoBoundingBox; }
	const Material& getMaterial() const { return m_material; }
	std::vector<Vertex>& getVertices() { return m_vertices; }
	std::vector<Vertex>& getBoundingBoxVertices() { return m_boundingboxvertices; }
	std::vector<GLint>& getIndices() { return m_indices; }
	std::vector<GLint>& getBoundingBoxIndices() { return m_boundingboxindices; }

	void setMaterial(const Material& _material);

	//drawing
	void setupVAOs();
	void setupBBVAOs();
	void drawBoundingBox();
	void drawMesh();
	void freeGLData();
	void freeBBGLData();
private:
	GLint m_id;
	bool m_hasBoundingBox;
	bool m_hasNormals;
	bool m_hasPositions;
	bool m_hasTexCoords;
	bool m_hasTangents;
	bool m_glinited;

	std::vector<Vertex> m_vertices;
	std::vector<Vertex> m_boundingboxvertices;
	std::vector<GLint> m_indices;
	std::vector<GLint> m_boundingboxindices;
	Material m_material;

	//bounding box bounds
	glm::vec3 m_bbcenter;
	glm::vec3 m_bbsize;
	GLfloat m_bbradius;

	//GL stuff
	GLuint m_posvbo;
	GLuint m_normvbo;
	GLuint m_uvvbo;
	GLuint m_tanvbo;
	GLuint m_ibo;
	
	GLuint m_vboBoundingBoxPos;
	GLuint m_iboBoundingBox;
	
	GLuint m_vao;
	GLuint m_vaoBoundingBox;
};

