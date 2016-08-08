#pragma once
#include "headers.h"
#include "Mesh.h"
#include "glerror.h"
class Model
{
public:
	Model();
	Model(const Model& _other);
	~Model();

	void addMesh(Mesh* _mesh);
	void addMeshes(std::vector<Mesh*>& _meshes);
	void removeMesh(GLint _id);

	//getters / setters
	std::vector<Mesh*>& getMeshes() { return m_meshes; }	

private:	
	std::vector<Mesh*> m_meshes;
};

