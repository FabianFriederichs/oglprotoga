#pragma once
#include "headers.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GameObject.h"
#include "RenderableGameObject.h"
#include "Model.h"
#include "Camera.h"
#include "Renderer.h"
#include <boost/heap/fibonacci_heap.hpp>
/*
	Keep track of Entity-Shader relations (Map with shader ID as key?)
	=> Mesh now has a Shader pointer
	=> rendering stuff, sorting by shaders ... will be done in Renderer class
	Render function to call every Shader's entities
	
	Keep track of camera

	Some sort of culling mechanism (occlusion culling, spatial subdivision, frustum culling ...) should be done in Scene class!

	#morestufftodo
*/


// Sorting meshes by shader:
//sort completely via heapsort (just at the beginning, nlogn is guaranteed)
//later sort by insertion via insertionsort. (for pre-sorted list the runtime is O(n) in best case)

class Scene
{
public:
	Scene();
	~Scene();

	void save();
	void load();
	void render();
protected:
	//scene data
	std::list<RenderableGameObject> m_gameobjects;	
	RenderList m_renderList;
	void addRenderable(RenderableGameObject& _renderable);
	void removeRenderable(GLint id);
	RenderList& getRenderList() { return m_renderList; };
	void createRenderList();


	std::list<Model> m_models;
	void addModel(Model& _model);
	/*std::list<Shader> m_shaders;
	void addShader(Shader& _shader);*/
	std::list<Texture> m_textures;
	void addTexture(Texture& _texture);
	std::list<DirectionalLight> m_directionallights;
	void addDirectionalLight(DirectionalLight& _directionallight);
	std::list<PointLight> m_pointlights;
	void addPointLight(PointLight& _pointlight);
	std::list<SpotLight> m_spotlights;
	void addSpotLight(SpotLight& _spotlight);
	std::list<Material> m_materials;
	void addMaterial(Material& _material);
	std::list<Camera> m_cameras;
	void addCamera(Camera& _camera);

	Renderer m_renderer;
	//inout


private:
	bool m_renderlistdirty;

	
};

