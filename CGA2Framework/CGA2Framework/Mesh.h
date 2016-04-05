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
	Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLint>& _indices, const Material& _material);
	Mesh(const Mesh& _other);
	~Mesh();

	void generateBoundingBox();
	bool hasBoundingBox() { return m_hasBoundingBox; }

	void addVertex(const Vertex& _vertex);
	void removeVertex(const Vertex& _vertex);
	void addIndex(const GLint _index);
	void removeIndex(const GLint _index);

	//getters / setters
	const GLint getID() const { return m_id; }
	GLint getVBOVertices(){ return m_vbo; }
	GLint getIBOIndices(){ return m_ibo; }
	GLint getVBOBoundingBox() { return m_vboBoundingBox; }
	GLint getIBOBoundingBox() { return m_iboBoundingBox; }
	GLint getVAOVertices() { return m_vao; }
	GLint getVAOBoundingBox() { return m_vaoBoundingBox; }
	Material& getMaterial() { return m_material; }
	std::vector<Vertex>& getVertices() { return m_vertices; }
	std::vector<Vertex>& getBoundingBoxVertices() { return m_boundingboxvertices; }
	std::vector<GLint>& getIndices() { return m_indices; }
	std::vector<GLint>& getBoundingBoxIndices() { return m_boundingboxindices; }

	void setMaterial(const Material& _material);

	//drawing
	void setupVAOs();
	void drawBoundingBox();
	void drawMesh();
	void freeGLData();
private:
	GLint m_id;
	bool m_hasBoundingBox;

	std::vector<Vertex> m_vertices;
	std::vector<Vertex> m_boundingboxvertices;
	std::vector<GLint> m_indices;
	std::vector<GLint> m_boundingboxindices;
	Material m_material;

	//GL stuff
	GLint m_vbo;
	GLint m_ibo;
	GLint m_vboBoundingBox;
	GLint m_iboBoundingBox;
	GLint m_vao;
	GLint m_vaoBoundingBox;
};

