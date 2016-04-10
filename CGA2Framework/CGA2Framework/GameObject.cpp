#include "GameObject.h"
#include "IDProvider.h"


GameObject::GameObject() :
m_active(true),
m_meshes(),
m_name(),
m_id(IDProvider::getInstance().createID()),
m_transform()
{
}


GameObject::~GameObject()
{
	m_meshes.clear();
}

void GameObject::addMesh(const Mesh& _mesh)
{
	m_meshes.push_back(_mesh);
}

void GameObject::addMeshes(const std::vector<Mesh>& _meshes)
{
	m_meshes.insert(m_meshes.end(), _meshes.begin(), _meshes.end());
}

void GameObject::removeMesh(const Mesh& _mesh)
{

}

void GameObject::draw()
{

}

void GameObject::drawBoundingBoxes()
{

}
