#pragma once
#include "headers.h"
#include "GameObject.h"
class Model : public GameObject
{
public:
	Model();
	Model(const Model& _other);
	~Model();

	void addMesh(const Mesh& _mesh);
	void addMeshes(const std::vector<Mesh>& _meshes);
	void removeMesh(const GLint _id);

	//getters / setters
	std::vector<Mesh>& getMeshes() { return m_meshes; }	

private:	
	std::vector<Mesh> m_meshes;
};

