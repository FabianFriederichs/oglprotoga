#include "Model.h"


Model::Model() :
m_meshes()
{
}

Model::Model(PRIMITIVETYPE _type):
m_meshes()
{
	addMesh(new Mesh(_type));
}

//Model::Model(const Model& _other) :
//m_meshes(_other.m_meshes)
//{
//
//}


Model::~Model()
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); it++)
		delete *it;
	m_meshes.clear();
}

void Model::addMesh(Mesh* _mesh)
{
	m_meshes.push_back(_mesh);
}

void Model::addMeshes(std::vector<Mesh*>& _meshes)
{
	m_meshes.insert(m_meshes.end(), _meshes.begin(), _meshes.end());
}

void Model::removeMesh(const GLint _id)
{
	m_meshes.erase(std::remove_if(m_meshes.begin(), m_meshes.end(), [_id](const Mesh* x)
	{ 
		if (x->getID() == _id)
		{
			if (x != nullptr)
				delete x;
			return true;
		}
		return false;;
	}), m_meshes.end());
}

