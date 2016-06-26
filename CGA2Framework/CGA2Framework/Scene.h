#pragma once
#include "headers.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GameObject.h"
#include "RenderableGameObject.h"
#include "Model.h"
#include "Camera.h"
#include "Renderer.h"
#include "ForwardShader.h"
#include "GPassShader.h"
#include "NthPassShader.h"
#include "OBJLoader.h"
#include "DDSLoader.h"
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
	Scene(GLint width, GLint height);
	~Scene();

	void save();
	void load();
	void render();
protected:
	//scene data
	std::list<RenderableGameObject*> m_gameobjects;	
	RenderList m_renderList;

	void addRenderable(RenderableGameObject* _renderable);
	void removeRenderable(GLint id);
	RenderList& getRenderList() { return m_renderList; };
	void createRenderList();


	std::list<Model*> m_models;
	void addModel(Model* _model);

	std::list<Texture*> m_textures;
	void addTexture(Texture* _texture);

	std::list<DirectionalLight*> m_directionallights;
	void addDirectionalLight(DirectionalLight* _directionallight);

	std::list<PointLight*> m_pointlights;
	void addPointLight(PointLight* _pointlight);

	std::list<SpotLight*> m_spotlights;
	void addSpotLight(SpotLight* _spotlight);

	std::list<Material*> m_materials;
	void addMaterial(Material* _material);
	
	Camera m_camera;

	ForwardShader m_forwardShader;
	GPassShader m_gpassShader;
	NthPassShader m_nthpassShader1;
	NthPassShader m_nthpassShader2;
	NthPassShader m_nthPassSHader3;
	

	Renderer m_renderer;

	//inout


private:
	bool m_renderlistdirty;
	GLint m_width;
	GLint m_height;

	
};

