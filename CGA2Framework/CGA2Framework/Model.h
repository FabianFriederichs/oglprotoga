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
	
	const std::string getPath(){ return m_path; }
	void setPath(const std::string &path){ m_path = path; }
	const std::string getName(){ return m_name; }
	void setName(const std::string &name){ m_name = name; }

private:	
	std::vector<Mesh*> m_meshes;
	std::string m_path;
	std::string m_name;
};

