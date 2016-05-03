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

Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices, const Material& _material, const bool _calcBoundingBox, Shader* _shader) :
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

void Mesh::setMaterial(const Material& _mat)
{
	m_material = _mat;
}

void Mesh::addVertex(const Vertex& _vertex)
{
	m_vertices.push_back(_vertex);
}

void Mesh::addIndicedVertex(const Vertex& _vertex)
{
	int index = -1;
	for (int i = 0; i < m_vertices.size(); i++)
	{
		if (m_vertices[i] == _vertex)
		{
			index = i;
			break;
		}
	}

	if (index > -1)	//vertex already exists
	{
		m_indices.push_back(index);
	}
	else
	{
		m_vertices.push_back(_vertex);			//add vertex
		m_indices.push_back(m_vertices.size() - 1);	//add index of last added vertex
	}
}

void Mesh::addIndex(const GLuint _index)
{
	m_indices.push_back(_index);
}

void Mesh::removeVertex(const Vertex& _vertex)
{
	m_vertices.erase(std::remove_if(m_vertices.begin(), m_vertices.end(), [&_vertex](const Vertex& x){return (x == _vertex); }), m_vertices.end());
}

void Mesh::removeIndex(const GLuint _index)
{
	m_indices.erase(std::remove_if(m_indices.begin(), m_indices.end(), [_index](const GLint x){return (x == _index); }), m_indices.end());
}

void Mesh::setupVAOs()
{
	//caching vertex data as plain float vector
	std::vector<GLfloat> buf;
	buf.reserve(m_vertices.size() * FLOATS_PER_VERTEX);
	for (auto it = m_vertices.begin(); it != m_vertices.end(); it++)
	{
		buf.push_back(it->getPosition().x);		//postition floats
		buf.push_back(it->getPosition().y);
		buf.push_back(it->getPosition().z);

		buf.push_back(it->getUV().x);			//uv floats
		buf.push_back(it->getUV().y);

		buf.push_back(it->getNormal().x);		//normal floats
		buf.push_back(it->getNormal().y);
		buf.push_back(it->getNormal().z);

		buf.push_back(it->getTangent().x);		//tangent floats	(bitangents not cached here because they can be easily calculated as tangent cross normal)
		buf.push_back(it->getTangent().y);
		buf.push_back(it->getTangent().z);
	}

	//glstuff

	if (m_vao == 0)
		glCreateVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * FLOATS_PER_VERTEX * sizeof(GLfloat), buf.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(GLfloat), reinterpret_cast<void*>(0)); //position format
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(GLfloat), reinterpret_cast<void*>(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(GLfloat), reinterpret_cast<void*>(8 * sizeof(GLfloat)));	

	glBindVertexArray(0);
}


void Mesh::setupBBVAOs()
{
	if (m_hasBoundingBox)
	{
		//caching vertex data as plain float vector
		std::vector<GLfloat> buf;
		buf.reserve(m_boundingboxvertices.size() * FLOATS_PER_VERTEX);
		for (auto it = m_boundingboxvertices.begin(); it != m_boundingboxvertices.end(); it++)
		{
			buf.push_back(it->getPosition().x);		//postition floats
			buf.push_back(it->getPosition().y);
			buf.push_back(it->getPosition().z);

			buf.push_back(it->getUV().x);			//uv floats
			buf.push_back(it->getUV().y);

			buf.push_back(it->getNormal().x);		//normal floats
			buf.push_back(it->getNormal().y);
			buf.push_back(it->getNormal().z);

			buf.push_back(it->getTangent().x);		//tangent floats	(bitangents not cached here because they can be easily calculated as tangent cross normal)
			buf.push_back(it->getTangent().y);
			buf.push_back(it->getTangent().z);
		}

		//glstuff
		if (m_bbvao == 0)
			glCreateVertexArrays(1, &m_bbvao);
		glBindVertexArray(m_bbvao);

		glGenBuffers(1, &m_bbvbo);
		glGenBuffers(1, &m_bbibo);

		glBindBuffer(GL_ARRAY_BUFFER, m_bbvbo);
		glBufferData(GL_ARRAY_BUFFER, m_boundingboxvertices.size() * FLOATS_PER_VERTEX * sizeof(GLfloat), buf.data(), GL_STATIC_DRAW); //vertices

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bbibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_boundingboxindices.size() * sizeof(GLuint), m_boundingboxindices.data(), GL_STATIC_DRAW);//indices

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(GLfloat), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(GLfloat), reinterpret_cast<void*>(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(GLfloat), reinterpret_cast<void*>(8 * sizeof(GLfloat)));

		glBindVertexArray(0);
		
	}
}

void Mesh::drawBoundingBox()
{
	if (m_bbvao != 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(m_bbvao);
		glDrawElements(GL_TRIANGLES, m_boundingboxindices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void Mesh::drawMesh()
{
	if (m_vao != 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void Mesh::freeGLData()
{
	

	if (m_vbo != 0)
	{
		glDeleteBuffers(1, &m_vbo);
	}

	if (m_ibo != 0)
	{
		glDeleteBuffers(1, &m_ibo);
	}

	if (m_vao != 0)
	{
		glDeleteVertexArrays(1, &m_vao);
	}
}

void Mesh::freeBBGLData()
{
	if (m_bbvbo != 0)
	{
		glDeleteBuffers(1, &m_bbvbo);
	}

	if (m_bbibo != 0)
	{
		glDeleteBuffers(1, &m_bbibo);
	}

	if (m_bbvao != 0)
	{
		glDeleteVertexArrays(1, &m_bbvao);
	}
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

void Mesh::generateNormals()
{
	for (int i = 0; i < m_vertices.size(); i++) //initialize all vertex normals with nullvectors
	{
		m_vertices[i].setNormal(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	for (int i = 0; i < m_indices.size(); i += 3)
	{
		glm::vec3 v1 = m_vertices[m_indices[i]].getPosition();
		glm::vec3 v2 = m_vertices[m_indices[i + 1]].getPosition();
		glm::vec3 v3 = m_vertices[m_indices[i + 2]].getPosition();

		//counter clockwise winding
		glm::vec3 edge1 = v2 - v1;
		glm::vec3 edge2 = v3 - v1;

		glm::vec3 normal = glm::cross(edge1, edge2);

		//for each vertex all corresponing normals are added. The result is a non unit length vector wich is the average direction of all assigned normals.
		m_vertices[m_indices[i]].m_normal += normal;
		m_vertices[m_indices[i + 1]].m_normal += normal;
		m_vertices[m_indices[i + 2]].m_normal += normal;
	}

	for (int i = 0; i < m_vertices.size(); i++)	//normalize all normals calculated in the previous step
	{
		m_vertices[i].m_normal = glm::normalize(m_vertices[i].m_normal);
	}

	m_hasNormals = true;
}

void Mesh::generateTangents()
{
	if (m_hasTexCoords)
	{
		//initialize tangents and bitangents with nullvecs
		for (int i = 0; i < m_vertices.size(); i++)
		{
			m_vertices[i].m_tangent = glm::vec3(0.0f, 0.0f, 0.0f);
			//m_vertices[i].m_bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		float det;
		glm::vec3 tangent;
		//glm::vec3 bitangent;
		glm::vec3 normal;

		//calculate and average tangents and bitangents just as we did when calculating the normals
		for (int i = 0; i < m_indices.size(); i += 3)
		{
			//3 vertices of a triangle
			glm::vec3 v1 = m_vertices[m_indices[i]].getPosition();			
			glm::vec3 v2 = m_vertices[m_indices[i + 1]].getPosition();			
			glm::vec3 v3 = m_vertices[m_indices[i + 2]].getPosition();

			//uvs
			glm::vec2 uv1 = m_vertices[m_indices[i]].getUV();
			glm::vec2 uv2 = m_vertices[m_indices[i + 1]].getUV();
			glm::vec2 uv3 = m_vertices[m_indices[i + 2]].getUV();

			//calculate edges in counter clockwise winding order
			glm::vec3 edge1 = v2 - v1;
			glm::vec3 edge2 = v3 - v1;

			//deltaus and deltavs
			glm::vec2 duv1 = uv2 - uv1;
			glm::vec2 duv2 = uv3 - uv1;

			det = duv1.x * duv2.y - duv2.x * duv1.y;

			if (fabs(det) < 1e-6f)		//if delta stuff is close to nothing ignore it
			{
				tangent = glm::vec3(1.0f, 0.0f, 0.0f);
				//bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				det = 1.0f / det;

				tangent.x = det * (duv2.y * edge1.x - duv1.y * edge2.x);
				tangent.y = det * (duv2.y * edge1.y - duv1.y * edge2.y);
				tangent.z = det * (duv2.y * edge1.z - duv1.y * edge2.z);

				//bitangent.x = det * (-duv2.x * edge1.x + duv1.x * edge2.x);
				//bitangent.y = det * (-duv2.x * edge1.y + duv1.x * edge2.y);
				//bitangent.z = det * (-duv2.x * edge1.z + duv1.x * edge2.z);
			}

			m_vertices[m_indices[i]].m_tangent += tangent;
			//m_vertices[m_indices[i]].m_bitangent += bitangent;

			m_vertices[m_indices[i + 1]].m_tangent += tangent;
			//m_vertices[m_indices[i + 1]].m_bitangent += bitangent;

			m_vertices[m_indices[i + 2]].m_tangent += tangent;
			//m_vertices[m_indices[i + 2]].m_bitangent += bitangent;
		}

		//orthogonalize and normalize tangents
		for (int i = 0; i < m_vertices.size(); i++)
		{
			Vertex* pv = &m_vertices[i];

			//normalize the stuff from before
			pv->m_normal = glm::normalize(pv->m_normal);
			pv->m_tangent = glm::normalize(pv->m_tangent);
			
			//gram schmidt reorthogonalize normal-tangent system
			pv->m_tangent = glm::normalize(pv->m_tangent - (glm::dot(pv->m_normal, pv->m_tangent) * pv->m_normal));
		}
		m_hasTangents = true;
	}
}

void Mesh::reverseWinding()	//swaps two indices of a triangle to reverse winding order
{
	for (int i = 0; i < m_indices.size(); i+=3)
	{
		GLuint tmp = m_indices[i + 1];
		m_indices[i + 1] = m_indices[i + 2];
		m_indices[i + 2] = tmp;
	}
}




