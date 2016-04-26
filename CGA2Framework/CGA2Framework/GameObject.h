#pragma once
#include "headers.h"
#include "Mesh.h"
#include "Transform.h"
class GameObject
{
public:
	GameObject();
	GameObject(const GameObject& _other);
	~GameObject();

	void addMesh(const Mesh& _mesh);	
	void addMeshes(const std::vector<Mesh>& _meshes);
	void removeMesh(const GLint _id);

	void draw(Shader& _shader);
	void drawBoundingBoxes(Shader& _shader);

	//getters / setters
	std::vector<Mesh>& getMeshes() { return m_meshes; }
	const bool isActive() const { return m_active; }
	const std::string& getName() const { return m_name; }
	const GLint getID() const { return m_id; }
	Transform& getTransform() { return m_transform; }

	void setName(const std::string& _name) { m_name = _name; }
	void setActive(const bool _active) { m_active = _active; }

private:
	bool m_active;
	std::vector<Mesh> m_meshes;
	std::string m_name;
	GLint m_id;
	Transform m_transform;
};

