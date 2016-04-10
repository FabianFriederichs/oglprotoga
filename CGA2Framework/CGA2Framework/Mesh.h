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
	Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices, const Material& _material, const bool _calcBoundingBox = false);
	Mesh(const Mesh& _other);
	~Mesh();

	void generateBoundingBox();
	
	const bool hasPositions() const { return m_hasPositions; }
	const bool hasBoundingBox() const { return m_hasBoundingBox; }
	const bool hasNormals() const { return m_hasNormals; }
	const bool hasTexCoords() const { return m_hasTexCoords; }
	const bool hasTangents() const { return m_hasTangents; }

	void addVertex(const Vertex& _vertex);
	void addIndicedVertex(const Vertex& _vertex);
	void removeVertex(const Vertex& _vertex);
	void addIndex(const GLuint _index);
	void removeIndex(const GLuint _index);	

	//getters / setters
	const GLuint getID() const { return m_id; }
	const GLuint getVBO() const { return m_vbo; }
	const GLuint getIBO() const { return m_ibo; }
	const GLuint getVBOBoundingBox() const { return m_bbvbo; }
	const GLuint getIBOBoundingBox() const { return m_bbibo; }
	const GLuint getVAOVertices() const { return m_vao; }
	const GLuint getVAOBoundingBox() const { return m_bbvao; }
	const Material& getMaterial() const { return m_material; }
	std::vector<Vertex>& getVertices() { return m_vertices; }
	std::vector<Vertex>& getBoundingBoxVertices() { return m_boundingboxvertices; }
	std::vector<GLuint>& getIndices() { return m_indices; }
	std::vector<GLuint>& getBoundingBoxIndices() { return m_boundingboxindices; }

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
	std::vector<GLuint> m_indices;
	std::vector<GLuint> m_boundingboxindices;
	Material m_material;

	//bounding box bounds
	glm::vec3 m_bbcenter;
	glm::vec3 m_bbsize;
	GLfloat m_bbradius;

	//GL stuff
	GLuint m_vbo;
	GLuint m_tanvbo;
	GLuint m_ibo;
	
	GLuint m_bbvbo;
	GLuint m_bbibo;
	
	GLuint m_vao;
	GLuint m_bbvao;
};

