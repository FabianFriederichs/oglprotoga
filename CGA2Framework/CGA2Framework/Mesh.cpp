#include "Mesh.h"


Mesh::Mesh() :
	m_id(IDProvider::getInstance().createID()),
	m_hasBoundingBox(false),
	m_vertices(),
	m_boundingboxvertices(),
	m_indices(),
	m_boundingboxindices(),
	m_material()
{
}

Mesh::Mesh(const Mesh& _other) :
	m_id(IDProvider::getInstance().createID()),
	m_hasBoundingBox(_other.m_hasBoundingBox),
	m_vertices(_other.m_vertices),
	m_boundingboxvertices(_other.m_boundingboxvertices),
	m_indices(_other.m_indices),
	m_boundingboxindices(_other.m_boundingboxindices),
	m_material(_other.m_material)
{
	
}

Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLint>& _indices, const Material& _material, const bool _calcBoundingBox) :
	m_id(IDProvider::getInstance().createID()),
	m_hasBoundingBox(false),
	m_vertices(_vertices),
	m_boundingboxvertices(),
	m_indices(_indices),
	m_boundingboxindices(),
	m_material(_material)
{
	if (_calcBoundingBox)
		generateBoundingBox();
}

Mesh::~Mesh()
{
	freeGLData();
}

void Mesh::addVertex(const Vertex& _vertex)
{
	m_vertices.push_back(_vertex);
}

void Mesh::addIndex(const GLint _index)
{
	m_indices.push_back(_index);
}

void Mesh::removeVertex(const Vertex& _vertex)
{
	m_vertices.erase(std::remove_if(m_vertices.begin(), m_vertices.end(), [&_vertex](const Vertex& x){return (x == _vertex); }), m_vertices.end());
}

void Mesh::removeIndex(const GLint _index)
{
	m_indices.erase(std::remove_if(m_indices.begin(), m_indices.end(), [_index](const GLint x){return (x == _index); }), m_indices.end());
}

void Mesh::setupVAOs()
{

}

void Mesh::drawBoundingBox()
{

}

void Mesh::drawMesh()
{

}

void Mesh::freeGLData()
{

}

void Mesh::generateBoundingBox()
{

}


