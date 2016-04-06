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
	if (m_vao == 0)
		glCreateVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//create vbo
	//bind vbo
	//arttribute pointer
	//create ibo
	//bind ibo

	glBindVertexArray(0);
}


void Mesh::setupBBVAOs()
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
	m_bbradius = std::max(std::max(m_bbsize.x, m_bbsize.y), m_bbsize.z) / 2.0f;

	//calc vertices/indices for drawing
	Vertex blb(m_bbcenter + glm::vec3(-(m_bbsize.x / 2.0f), -(m_bbsize.y / 2.0f), -(m_bbsize.z / 2.0f)));	//0	//bottom-left-back
	Vertex brb(m_bbcenter + glm::vec3((m_bbsize.x / 2.0f), -(m_bbsize.y / 2.0f), -(m_bbsize.z / 2.0f)));	//1	//bottom-right-back
	Vertex blf(m_bbcenter + glm::vec3(-(m_bbsize.x / 2.0f), -(m_bbsize.y / 2.0f), (m_bbsize.z / 2.0f)));	//2	//bottom-left-front
	Vertex brf(m_bbcenter + glm::vec3((m_bbsize.x / 2.0f), -(m_bbsize.y / 2.0f), (m_bbsize.z / 2.0f)));		//3	//bottom-right-front
	Vertex tlb(m_bbcenter + glm::vec3(-(m_bbsize.x / 2.0f), (m_bbsize.y / 2.0f), -(m_bbsize.z / 2.0f)));	//4	//top-left-back
	Vertex trb(m_bbcenter + glm::vec3((m_bbsize.x / 2.0f), (m_bbsize.y / 2.0f), -(m_bbsize.z / 2.0f)));		//5	//top-right-back
	Vertex tlf(m_bbcenter + glm::vec3(-(m_bbsize.x / 2.0f), (m_bbsize.y / 2.0f), -(m_bbsize.z / 2.0f)));	//6	//top-left-front
	Vertex trf(m_bbcenter + glm::vec3((m_bbsize.x / 2.0f), (m_bbsize.y / 2.0f), -(m_bbsize.z / 2.0f)));		//7	//top-right-front

	m_boundingboxvertices.push_back(blb);
	m_boundingboxvertices.push_back(brb);
	m_boundingboxvertices.push_back(blf);
	m_boundingboxvertices.push_back(brf);
	m_boundingboxvertices.push_back(tlb);
	m_boundingboxvertices.push_back(trb);
	m_boundingboxvertices.push_back(tlf);
	m_boundingboxvertices.push_back(trf);

	//setup indices	
	int indic[] = { 1, 3, 2,
					2, 0, 1,
					1, 0, 4,
					4, 5, 1,
					4, 0, 6,
					6, 0, 2,
					1, 5, 3,
					3, 5, 7,
					6, 5, 4,
					7, 5, 6,
					7, 6, 2,
					2, 3, 7 };

	//clear and push to vertex list / index list
	m_boundingboxindices.clear();
	m_boundingboxindices.insert(m_boundingboxindices.begin(), indic, indic + 36);

	m_hasBoundingBox = true;
}


