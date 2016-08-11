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
	Texture2D* wooddiff = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\wooddiff_RGBA32UI.dds"));
	Texture2D* woodspec = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\woodspec_RGBA32UI.dds"));
	Texture2D* woodgloss = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\woodgloss_RGBA32UI.dds"));
	Texture2D* woodnormal = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\woodnormal_RGBA32UI.dds"));
	Texture2D* woodheight = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\woodheight_RGBA32UI.dds"));
	Texture2D* metaldiff = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metaldiff_RGBA32UI.dds"));
	Texture2D* metalspec = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metalspec_RGBA32UI.dds"));
	Texture2D* metalgloss = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metalgloss_RGBA32UI.dds"));
	Texture2D* metalnormal = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metalnormal_RGBA32UI.dds"));
	Texture2D* metalheight = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metalheight_RGBA32UI.dds"));
	//this thing will be forward shaded
	ForwardShader* forward = new ForwardShader();
	forward->load("BaseVertex.vert", "BaseFrag.frag");

	addTexture(wooddiff);
	addTexture(woodspec);
	addTexture(woodgloss);
	addTexture(woodnormal);
	addTexture(woodheight);
	addTexture(metaldiff);
	addTexture(metalspec);
	addTexture(metalgloss);
	addTexture(metalnormal);
	addTexture(metalheight);

	//load wood and metal material
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
	//2 submeshes
	(*mshit++)->setMaterial(wood);
	(*mshit)->setMaterial(metal);

	//create an instance of this model and add it to the scene gameobjects
	RenderableGameObject* go = new RenderableGameObject();
	go->setModel(*moit);
	go->getTransform().setTranslate(glm::vec3(0.0f, 0.0f, -2.0f));

	addRenderable(go);

	//create a directional light
	DirectionalLight* dirlight = new DirectionalLight(glm::vec3(1.0f, -1.0f, -1.0f), Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f));
	addDirectionalLight(dirlight);

	PointLight* pointlight = new PointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),1.0f, 0.7f, 1.8f, 7.0f);
	addPointLight(pointlight);
	//create a flycam
	FPSCamera* cam = new FPSCamera(45.0f, 800, 600, 0.1f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	cam->Fly(true);
	cam->recalcProj();
	m_camera = cam;

	int dummy = 0;
}

void TestScene::save()
{

}


