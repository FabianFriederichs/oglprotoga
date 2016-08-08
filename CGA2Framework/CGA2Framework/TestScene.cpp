#include "TestScene.h"


TestScene::TestScene(GLint width, GLint height) : Scene(width, height)
{
}


TestScene::~TestScene()
{
}

void TestScene::load()
{

	//load testmodel "uglypot"
	std::list<Model*> models = OBJLoader::loadOBJ("Assets\\Models\\uglypot.obj");
	GLERR
	Texture* wooddiff = DDSLoader::loadDDSTex("Assets\\Materials\\wooddiff.dds");
	GLERR
	Texture* woodspec = DDSLoader::loadDDSTex("Assets\\Materials\\woodspec.dds");
	GLERR
	Texture* woodgloss = DDSLoader::loadDDSTex("Assets\\Materials\\woodgloss.dds");
	GLERR
	Texture* woodnormal = DDSLoader::loadDDSTex("Assets\\Materials\\woodnormal.dds");
	GLERR
	Texture* woodheight = DDSLoader::loadDDSTex("Assets\\Materials\\woodheight.dds");
	GLERR
	Texture* metaldiff = DDSLoader::loadDDSTex("Assets\\Materials\\metaldiff.dds");
	GLERR
	Texture* metalspec = DDSLoader::loadDDSTex("Assets\\Materials\\metalspec.dds");
	GLERR
	Texture* metalgloss = DDSLoader::loadDDSTex("Assets\\Materials\\metalgloss.dds");
	GLERR
	Texture* metalnormal = DDSLoader::loadDDSTex("Assets\\Materials\\metalnormal.dds");
	GLERR
	Texture* metalheight = DDSLoader::loadDDSTex("Assets\\Materials\\metalheight.dds");
	GLERR
	//this thing will be forward shaded
	ForwardShader* forward = new ForwardShader();
	GLERR
	forward->load("BaseVertex.vert", "BaseFrag.frag");
	GLERR

	//load wood and metal material
	Material* wood = new Material();
	GLERR
	wood->addTexture(wooddiff);
	GLERR
	wood->addTexture(woodspec);
	GLERR
	wood->addTexture(woodgloss);
	GLERR
	wood->addTexture(woodnormal);
	GLERR
	wood->addTexture(woodheight);
	GLERR
	wood->setShader(forward);
	GLERR
	Material* metal = new Material();
	GLERR
	metal->addTexture(metaldiff);
	GLERR
	metal->addTexture(metalspec);
	GLERR
	metal->addTexture(metalgloss);
	GLERR
	metal->addTexture(metalnormal);
	GLERR
	metal->addTexture(metalheight);
	GLERR
	metal->setShader(forward);
	GLERR
	std::list<Model*>::iterator moit = models.begin();
	GLERR
	std::vector<Mesh*>::iterator mshit = (*moit)->getMeshes().begin();
	GLERR
	//2 submeshes
	(*mshit++)->setMaterial(wood);
	GLERR
	(*mshit)->setMaterial(metal);
	GLERR

	//create an instance of this model and add it to the scene gameobjects
	RenderableGameObject* go = new RenderableGameObject();
	GLERR
	go->setModel(*moit);
	GLERR
	go->getTransform().setTranslate(glm::vec3(0.0f, 0.0f, -2.0f));
	GLERR

	addRenderable(go);
	GLERR

	//create a directional light
	DirectionalLight* dirlight = new DirectionalLight(glm::vec3(1.0f, -1.0f, -1.0f), Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f));
	GLERR
	addDirectionalLight(dirlight);
	GLERR

	//create a flycam
	FPSCamera* cam = new FPSCamera(45.0f, 800, 600, 0.1f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	GLERR
	cam->Fly(true);
	cam->recalcProj();
	GLERR
	m_camera = cam;
	GLERR

	int dummy = 0;
}

void TestScene::save()
{

}


