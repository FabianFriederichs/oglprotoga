#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::render()
{
	m_renderer.renderforward(m_renderList);
}

void Scene::addRenderable(RenderableGameObject& _renderable)
{
	m_gameobjects.push_back(_renderable);
}

void Scene::removeRenderable(GLint _id)
{
	/*auto it = std::find_if(std::begin(m_gameobjects),
		std::end(m_gameobjects),
		[&](const RenderableGameObject v){ return v.getID() == _id; });*/
	m_renderList.erase(std::remove_if(m_renderList.begin(), m_renderList.end(), [_id](const RenderPair& x){ return x.second->getID() == _id; }), m_renderList.end());
	m_gameobjects.erase(std::remove_if(m_gameobjects.begin(), m_gameobjects.end(), [_id](const RenderableGameObject& v){ return v.getID() == _id; }), m_gameobjects.end());

}

void Scene::addModel(Model& _model)
{
	m_models.push_back(_model);
}

void Scene::addMaterial(Material& _material)
{
	m_materials.push_back(_material);
}

void Scene::addTexture(Texture& _texture)
{
	m_textures.push_back(_texture);
}

void Scene::addDirectionalLight(DirectionalLight& _dirlight)
{
	m_directionallights.push_back(_dirlight);
}

void Scene::addPointLight(PointLight& _pointlight)
{
	m_pointlights.push_back(_pointlight);
}

void Scene::addSpotLight(SpotLight& _spotLight)
{
	m_spotlights.push_back(_spotLight);
}


void Scene::createRenderList()
{
	m_renderList.clear();
	for each(auto v in m_gameobjects)
	{
		for each(auto m in v.getModel()->getMeshes())
		{
			m_renderList.push_back(std::pair<Mesh*, RenderableGameObject*>(&m, &v));
		}
	}
	m_renderList.sort([](const std::pair<Mesh*, RenderableGameObject*>& lhs, const std::pair<Mesh*, RenderableGameObject*>& rhs) {
		return lhs.first->getShader()->getID() < rhs.first->getShader()->getID();
	});
}
