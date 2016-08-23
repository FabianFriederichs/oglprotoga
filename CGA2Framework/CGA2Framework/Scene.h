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
#include "Billboard.h"
#include "util.h"
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

	virtual void save(){};
	virtual void load(){};
	virtual void save(std::string _path);
	virtual void load(std::string _path);
	virtual void clear();
	//void render();
	//scene data
	std::list<RenderableGameObject*> m_gameobjects;	
	//RenderList m_renderList;

	void addRenderable(RenderableGameObject* _renderable);
	void removeRenderable(GLint id);
	//RenderList& getRenderList() { return m_renderList; };
	//void createRenderList();

	std::list<Shader*> m_shaders;
	void addShader(Shader* _shader);

	std::list<Model*> m_models;
	void addModel(Model* _model);

	std::list<Texture2D*> m_textures;
	void addTexture(Texture2D* _texture);

	std::list<TextureCB*> m_cubemaps;
	void addCubeMap(TextureCB* _texture);

	std::list<DirectionalLight*> m_directionallights;
	void addDirectionalLight(DirectionalLight* _directionallight);

	std::list<PointLight*> m_pointlights;
	void addPointLight(PointLight* _pointlight);

	std::list<SpotLight*> m_spotlights;
	void addSpotLight(SpotLight* _spotlight);

	std::list<Material*> m_materials;
	void addMaterial(Material* _material);

	std::list<Billboard*> m_billboards;
	void addBillboard(Billboard* _billboard);

	Camera* m_camera;
	

	//Renderer m_renderer;

	//inout

	//bool m_renderlistdirty;
	GLint m_width;
	GLint m_height;

	/*
	file format:
	/resources
	\t/textures
	\t\t<name>path
	\t/shaders
	\t\t<name><Vpath><Fpath>
	\t/models
	\t\t<name>path

	/materials
	\tname
	\t\tshadername
	\t\t/textures
	\t\t\ttexturename
	...
	/submesh-material
	\t<modelname>
	\t\t<submeshid>materialname
	/go
	\tmodelname<scale><rotation><translation>
	/dirlights
	\t<dir><scale><rotate><translate><color>
	/plights
	\t<color><scale><rotate><translate><constant><lineear><quadratic><range>
	/slights
	\t<color><scale><rotate><translate><dir><constannt>linear><quadratic><range>
	/camera
	\ttype
	\t<fov><width><height><znear><zfar><wup><wright><wforw>
	\tfly(true/false)
	/width
	\t<>
	/height
	\t<>
	*/

};

