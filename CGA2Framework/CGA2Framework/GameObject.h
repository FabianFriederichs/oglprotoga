#pragma once
#include "headers.h"
#include "Mesh.h"
#include "Transform.h"
class GameObject
{
public:
	GameObject();
	~GameObject();

	void addMesh(const Mesh& _mesh);
	void addMeshes(const std::vector<Mesh>& _meshes);
	void removeMesh(const Mesh& _mesh);

	void draw();
	void drawBoundingBoxes();

private:
	bool m_active;
	std::vector<Mesh> m_meshes;
	std::string m_name;
	GLint m_id;
	Transform m_transform;
};

