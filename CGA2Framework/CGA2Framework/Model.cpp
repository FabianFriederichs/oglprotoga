#include "Model.h"


Model::Model() : GameObject(),
m_meshes()
{
}

Model::Model(const Model& _other) : GameObject(_other),
m_meshes(_other.m_meshes)
{

}


Model::~Model()
{
	m_meshes.clear();
}

void Model::addMesh(const Mesh& _mesh)
{
	m_meshes.push_back(_mesh);
}

void Model::addMeshes(const std::vector<Mesh>& _meshes)
{
	m_meshes.insert(m_meshes.end(), _meshes.begin(), _meshes.end());
}

void Model::removeMesh(const GLint _id)
{
	m_meshes.erase(std::remove_if(m_meshes.begin(), m_meshes.end(), [_id](const Mesh& x){ return x.getID() == _id; }), m_meshes.end());
}

