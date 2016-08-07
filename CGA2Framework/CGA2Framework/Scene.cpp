#include "Scene.h"


Scene::Scene() :
m_camera(45.f, 1920.0f, 1080.0f, 0.1f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f)),
m_directionallights(),
m_pointlights(),
m_spotlights(),
m_gameobjects(),
m_materials(),
m_models(),
m_renderer(),
m_renderList(),
m_textures(),
m_forwardShader(),
m_gpassShader(),
m_nthpassShader1(),
m_nthpassShader2(),
m_nthPassSHader3()
{
	
}

Scene::Scene(GLint width, GLint height) :
m_camera(45.f, width, height, 0.1f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f)),
m_directionallights(),
m_pointlights(),
m_spotlights(),
m_gameobjects(),
m_materials(),
m_models(),
m_renderer(),
m_renderList(),
m_textures(),
m_forwardShader(),
m_gpassShader(),
m_nthpassShader1(),
m_nthpassShader2(),
m_nthPassSHader3(),
m_height(height),
m_width(width)
{

}


Scene::~Scene()
{
}

void Scene::load()
{
	std::list<Model*> models = OBJLoader::loadOBJ("Assets\\Models\\uglypot.obj");
	Texture* wooddiff = DDSLoader::loadDDSTex("Assets\\Materials\\wooddiff.dds");
	Texture* woodspec = DDSLoader::loadDDSTex("Assets\\Materials\\woodspec.dds");
	Texture* woodgloss = DDSLoader::loadDDSTex("Assets\\Materials\\woodgloss.dds");
	Texture* woodnormal = DDSLoader::loadDDSTex("Assets\\Materials\\woodnormal.dds");
	Texture* woodheight = DDSLoader::loadDDSTex("Assets\\Materials\\woodheight.dds");
	Texture* metaldiff = DDSLoader::loadDDSTex("Assets\\Materials\\metaldiff.dds");
	Texture* metalspec = DDSLoader::loadDDSTex("Assets\\Materials\\metalspec.dds");
	Texture* metalgloss = DDSLoader::loadDDSTex("Assets\\Materials\\metalgloss.dds");
	Texture* metalnormal = DDSLoader::loadDDSTex("Assets\\Materials\\metalnormal.dds");
	Texture* metalheight = DDSLoader::loadDDSTex("Assets\\Materials\\metalheight.dds");
	ForwardShader* forward = new ForwardShader();
	forward->load("BaseVertex.vert", "BaseFrag.frag");
	Material* wood = new Material();
	wood->addTexture(wooddiff);
	wood->addTexture(woodspec);
	wood->addTexture(woodgloss);
	wood->addTexture(woodnormal);
	wood->addTexture(woodheight);
	wood->setShader(forward);
	Material* metal = new Material();
	metal->addTexture(metaldiff);
	metal->addTexture(metalspec);
	metal->addTexture(metalgloss);
	metal->addTexture(metalnormal);
	metal->addTexture(metalheight);
	metal->setShader(forward);
	std::list<Model*>::iterator moit = models.begin();
	std::vector<Mesh*>::iterator mshit = (*moit)->getMeshes().begin();
	(*mshit++)->setMaterial(wood);
	(*mshit)->setMaterial(metal);

	RenderableGameObject* go = new RenderableGameObject();
	go->setModel(*moit);
	go->getTransform().setTranslate(glm::vec3(0.0f, 0.0f, -2.0f));
	int dummy = 0;
}

void Scene::save()
{

}

void Scene::render()
{
	m_renderer.renderforward(m_renderList, &m_camera, m_directionallights, m_pointlights, m_spotlights);
}

void Scene::addRenderable(RenderableGameObject* _renderable)
{
	m_gameobjects.push_back(_renderable);

	for (std::vector<Mesh*>::iterator mit = _renderable->getModel()->getMeshes().begin(); mit != _renderable->getModel()->getMeshes().end(); mit = mit++)
	{
		RenderList::iterator insertpos = m_renderList.begin();
		while (insertpos != m_renderList.end() && (*mit)->getMaterial()->getID() < insertpos->first->getMaterial()->getID())
		{
			insertpos++;
		}
		m_renderList.insert(insertpos, RenderPair(*mit, _renderable));
	}
}

void Scene::removeRenderable(GLint _id)
{
	/*auto it = std::find_if(std::begin(m_gameobjects),
		std::end(m_gameobjects),
		[&](const RenderableGameObject v){ return v.getID() == _id; });*/
	m_renderList.erase(std::remove_if(m_renderList.begin(), m_renderList.end(), [_id](const RenderPair& x)
	{
		if (x.second->getID() == _id)
		{
			return true;
		}
		return false;
	}), m_renderList.end());

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

void Scene::addTexture(Texture* _texture)
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


void Scene::createRenderList()
{
	m_renderList.clear();
	for each(auto v in m_gameobjects)
	{
		for each(auto m in v->getModel()->getMeshes())
		{
			m_renderList.push_back(RenderPair(m, v));
		}
	}
	m_renderList.sort([](const std::pair<Mesh*, RenderableGameObject*>& lhs, const std::pair<Mesh*, RenderableGameObject*>& rhs) {
		return lhs.first->getMaterial()->getID() < rhs.first->getMaterial()->getID();
	});
}
