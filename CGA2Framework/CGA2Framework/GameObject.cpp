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

GameObject::GameObject(const GameObject& _other) :
	m_active(_other.m_active),
	m_meshes(_other.m_meshes),
	m_name(_other.m_name),
	m_id(IDProvider::getInstance().createID()),
	m_transform(_other.m_transform)
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

void GameObject::removeMesh(const GLint _id)
{
	m_meshes.erase(std::remove_if(m_meshes.begin(), m_meshes.end(), [_id](const Mesh& x){ return x.getID() == _id; }), m_meshes.end());
}

void GameObject::draw()
{
	//draw all meshes
	if (m_active)
	{
		for (auto mesh = m_meshes.begin(); mesh != m_meshes.end(); mesh++)
		{
			mesh->drawMesh();
		}
	}
}

void GameObject::drawBoundingBoxes()
{
	//draw all boundingboxes
	if (m_active)
	{
		for (auto mesh = m_meshes.begin(); mesh != m_meshes.end(); mesh++)
		{
			mesh->drawBoundingBox();
		}
	}
}
