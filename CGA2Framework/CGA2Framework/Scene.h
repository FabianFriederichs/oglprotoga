#pragma once
#include "headers.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GameObject.h"
#include "RenderableGameObject.h"
#include "Model.h"
#include "Camera.h"
#include "FPSCamera.h"
//#include "Renderer.h"
#include "ForwardShader.h"
#include "GPassShader.h"
#include "NthPassShader.h"
#include "OBJLoader.h"
#include "DDSLoader.h"
#include "glerror.h"
#include "Texture2D.h"
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
	virtual ~Scene();

	virtual void save() = 0;
	virtual void load() = 0;
	virtual void clear();
	//void render();
	//scene data
	std::list<RenderableGameObject*> m_gameobjects;	
	//RenderList m_renderList;

	void addRenderable(RenderableGameObject* _renderable);
	void removeRenderable(GLint id);
	//RenderList& getRenderList() { return m_renderList; };
	//void createRenderList();


	std::list<Model*> m_models;
	void addModel(Model* _model);

	std::list<Texture2D*> m_textures;
	void addTexture(Texture2D* _texture);

	std::list<DirectionalLight*> m_directionallights;
	void addDirectionalLight(DirectionalLight* _directionallight);

	std::list<PointLight*> m_pointlights;
	void addPointLight(PointLight* _pointlight);

	std::list<SpotLight*> m_spotlights;
	void addSpotLight(SpotLight* _spotlight);

	std::list<Material*> m_materials;
	void addMaterial(Material* _material);

	std::list<Shader*> m_shaders;
	void addShader(Shader* _shader);
	
	Camera* m_camera;
	

	//Renderer m_renderer;

	//inout

	//bool m_renderlistdirty;
	GLint m_width;
	GLint m_height;

	
};

