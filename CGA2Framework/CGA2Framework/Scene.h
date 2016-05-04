#pragma once
#include "headers.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GameObject.h"
#include "Camera.h"
/*
	Keep track of Entity-Shader relations (Map with shader ID as key?)
	=> Mesh now has a Shader pointer
	=> rendering stuff, sorting by shaders ... will be done in Renderer class
	Render function to call every Shader's entities
	
	Keep track of camera

	Some sort of culling mechanism (occlusion culling, spatial subdivision, frustum culling ...) should be done in Scene class!

	#morestufftodo
*/
class Scene
{
public:
	Scene();
	~Scene();

	virtual void render() = 0;
	void save(const std::string& _filepath);
	void load(const std::string& _filepath);

protected:
	std::vector<GameObject> m_gameobjects;
	std::vector<Shader> m_shaders;
	std::vector<DirectionalLight> m_directionallights;
	std::vector<PointLight> m_pointlights;
	std::vector<SpotLight> m_spotlights;
	std::vector<Material> m_materials;
	std::vector<Camera> m_cameras;
};

