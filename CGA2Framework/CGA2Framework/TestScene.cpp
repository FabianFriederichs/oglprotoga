#include "TestScene.h"


TestScene::TestScene(GLint width, GLint height) : Scene(width, height)
{
}


TestScene::~TestScene()
{
}

void TestScene::load()
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

void TestScene::save()
{

}


