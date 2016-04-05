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

void Mesh::setupVAO()
{

}

void Mesh::setupBBVAO()
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

void Mesh::freeBBGLData()
{

}

void Mesh::generateBoundingBox()
{
	//TODO: improve later to fit bb closely to mesh via appropriate rotation
	//calc bounds
	float xMax = std::numeric_limits<float>::min();
	float yMax = std::numeric_limits<float>::min();
	float zMax = std::numeric_limits<float>::min();
	float xMin = std::numeric_limits<float>::max();
	float yMin = std::numeric_limits<float>::max();
	float zMin = std::numeric_limits<float>::max();
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	int numVerts = m_vertices.size();
	for (int i = 0; i < numVerts; ++i)
	{
		x = m_vertices[i].getPosition().x;
		y = m_vertices[i].getPosition().y;
		z = m_vertices[i].getPosition().z;
		if (x < xMin) xMin = x;
		if (x > xMax) xMax = x;
		if (y < yMin) yMin = y;
		if (y > yMax) yMax = y;
		if (z < zMin) zMin = z;
		if (z > zMax) zMax = z;
	}
	m_bbcenter = glm::vec3(((xMin + xMax) / 2.0f), ((yMin + yMax) / 2.0f), ((zMin + zMax) / 2.0f));
	m_bbsize = glm::vec3(xMax - xMin, yMax - yMin, zMax - zMin);
	m_bbradius = std::max(std::max(m_bbsize.x, m_bbsize.y), m_bbsize.z);

	//calc vertices/indices for drawing
	Vertex blb(m_bbcenter + glm::vec3(-(m_bbsize.x / 2.0f), -(m_bbsize.y / 2.0f), (m_bbsize.z / 2.0f)));	//bottom-left-back
	Vertex brb();	//bottom-right-back
	Vertex blf();	//bottom-left-front
	Vertex brf();	//bottom-right-front
	Vertex tlb();	//top-left-back
	Vertex trb();	//top-right-back
	Vertex tlf();	//top-left-front
	Vertex trf();	//top-right-front
}


