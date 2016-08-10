#include "Scene.h"


Scene::Scene() :
m_camera(0),
m_directionallights(),
m_pointlights(),
m_spotlights(),
m_gameobjects(),
m_materials(),
m_models(),
//m_renderer(),
//m_renderList(),
m_textures(),
m_shaders()
{
	
}

Scene::Scene(GLint width, GLint height) :
m_camera(0),
m_directionallights(),
m_pointlights(),
m_spotlights(),
m_gameobjects(),
m_materials(),
m_models(),
//m_renderer(),
//m_renderList(),
m_textures(),
m_height(height),
m_width(width),
m_shaders()
{

}


Scene::~Scene()
{
	//clear();
}

//void Scene::render()
//{
//	//m_renderer.renderforward(m_renderList, &m_camera, m_directionallights, m_pointlights, m_spotlights);
//}

void Scene::addRenderable(RenderableGameObject* _renderable)
{
	m_gameobjects.push_back(_renderable);

	/*for (std::vector<Mesh*>::iterator mit = _renderable->getModel()->getMeshes().begin(); mit != _renderable->getModel()->getMeshes().end(); mit = mit++)
	{
		RenderList::iterator insertpos = m_renderList.begin();
		while (insertpos != m_renderList.end() && (*mit)->getMaterial()->getID() < insertpos->first->getMaterial()->getID())
		{
			insertpos++;
		}
		m_renderList.insert(insertpos, RenderPair(*mit, _renderable));
	}*/
}

void Scene::removeRenderable(GLint _id)
{
	/*auto it = std::find_if(std::begin(m_gameobjects),
		std::end(m_gameobjects),
		[&](const RenderableGameObject v){ return v.getID() == _id; });*/
	/*m_renderList.erase(std::remove_if(m_renderList.begin(), m_renderList.end(), [_id](const RenderPair& x)
	{
		if (x.second->getID() == _id)
		{
			return true;
		}
		return false;
	}), m_renderList.end());*/

	m_gameobjects.erase(std::remove_if(m_gameobjects.begin(), m_gameobjects.end(), [_id](const RenderableGameObject* v)
	{ 
		if (v->getID() == _id)
		{
			if (v != nullptr)
				delete v;
			return true;
		}
		return false;
	}), m_gameobjects.end());

}


void Scene::addModel(Model* _model)
{
	m_models.push_back(_model);
}

void Scene::addMaterial(Material* _material)
{
	m_materials.push_back(_material);
}

void Scene::addTexture(Texture2D* _texture)
{
	m_textures.push_back(_texture);
}

void Scene::addDirectionalLight(DirectionalLight* _dirlight)
{
	m_directionallights.push_back(_dirlight);
}

void Scene::addPointLight(PointLight* _pointlight)
{
	m_pointlights.push_back(_pointlight);
}

void Scene::addSpotLight(SpotLight* _spotLight)
{
	m_spotlights.push_back(_spotLight);
}

void Scene::addShader(Shader* _shader)
{
	m_shaders.push_back(_shader);
}


void Scene::clear()
{
	//clear all the object collections in scene
	for each(GameObject* g in m_gameobjects)
	{
		if (g != nullptr)
			delete g;
	}
	m_gameobjects.clear();

	for each(DirectionalLight* l in m_directionallights)
	{
		if (l != nullptr)
			delete l;
	}
	m_directionallights.clear();

	for each(PointLight* l in m_pointlights)
	{
		if (l != nullptr)
			delete l;
	}
	m_pointlights.clear();

	for each(SpotLight* l in m_spotlights)
	{
		if (l != nullptr)
			delete l;
	}
	m_spotlights.clear();

	for each(Model* m in m_models)
	{
		if (m != nullptr)
			delete m;
	}
	m_models.clear();

	for each(Material* m in m_materials)
	{
		if (m != nullptr)
			delete m;
	}
	m_materials.clear();

	for each(Texture2D* t in m_textures)
	{
		if (t != nullptr)
			delete t;
	}
	m_textures.clear();

	for each(Shader* s in m_shaders)
	{
		if (s != nullptr)
			delete s;
	}
	m_shaders.clear();

	if (m_camera != nullptr)
		delete m_camera;
}

//
//void Scene::createRenderList()
//{
//	m_renderList.clear();
//	for each(auto v in m_gameobjects)
//	{
//		for each(auto m in v->getModel()->getMeshes())
//		{
//			m_renderList.push_back(RenderPair(m, v));
//		}
//	}
//	m_renderList.sort([](const std::pair<Mesh*, RenderableGameObject*>& lhs, const std::pair<Mesh*, RenderableGameObject*>& rhs) {
//		return lhs.first->getMaterial()->getID() < rhs.first->getMaterial()->getID();
//	});
//}
