#include "MainGame.h"


MainGame::MainGame(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver, const GLint msaasamples)
	: GameWindow(sizex, sizey, cvmaj, cvmin, title, uselatestglver, msaasamples)
{
	m_curmousex = 0.0;
	m_curmousey = 0.0;
	for (auto begin = std::begin(keys), end = std::end(keys); begin != end; ++begin) *begin = false;
}


MainGame::~MainGame()
{
	delete m_scene;
	delete m_scenerenderer;
}

void MainGame::keycallback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
	
}

void MainGame::mmcallback(double xpos, double ypos)
{
	//fprintf(stderr, "Mousepos: X: %.0f Y: %.0f\n", xpos, ypos);
	if (m_scene->m_camera != nullptr)
	{
		m_scene->m_camera->Rotate(glm::vec3((xpos - m_curmousex) * 0.002, (ypos - m_curmousey) * 0.002, 0.0));
		m_curmousex = xpos;
		m_curmousey = ypos;
	}
}
void MainGame::mbcallback(int button, int action, int mods)
{

}

void MainGame::mscrcallback(double xoffset, double yoffset)
{

}

GLvoid MainGame::update(GLdouble time)
{
	(*m_scene->m_pointlights.begin())->getTransform().setTranslate(m_scene->m_camera->GetPosition());
	if (m_scene->m_camera != nullptr)
	{
		MoveData md = { vec3(0, 0, 0), 0 };

		md.Multiplier = 0.05f;
		if (keys[GLFW_KEY_W])
			md.mtype += vec3(1, 0, 0);
		if (keys[GLFW_KEY_S])
			md.mtype += vec3(-1, 0, 0);
		if (keys[GLFW_KEY_A])
			md.mtype += vec3(0, 0, 1);
		if (keys[GLFW_KEY_D])
			md.mtype += vec3(0, 0, -1);
		if (md.mtype != vec3(0, 0, 0))
		{
		m_scene->m_camera->Move(md);
		
	}
		for (auto b : m_scene->m_billboards)
			b->Orient();
		md = { vec3(0, 0, 0), 0 };

		md.Multiplier = 0.05f;
		if (keys[GLFW_KEY_UP])
			md.mtype += vec3(1, 0, 0);
		if (keys[GLFW_KEY_DOWN])
			md.mtype += vec3(-1, 0, 0);
		if (keys[GLFW_KEY_LEFT])
			md.mtype += vec3(0, 0, -1);
		if (keys[GLFW_KEY_RIGHT])
			md.mtype += vec3(0, 0, 1);
		if (keys[GLFW_KEY_SPACE])
			md.mtype += vec3(0, 1, 0);
		if (keys[GLFW_KEY_V])
			md.mtype += vec3(0, -1, 0);

		if (md.mtype != vec3(0, 0, 0))
			m_scene->m_renderables.find(OPAQUE)->second->getTransform().translate(md.mtype*md.Multiplier);

		for (auto go : m_scene->m_gameobjects)
			go->getTransform().rotate(vec3(0.001f*(((abs((int)(go->getTransform().getTranslate().z) + abs((int)(go->getTransform().getTranslate().y)))) % 6) - 3), 0.001f*(((abs((int)(go->getTransform().getTranslate().z) + abs((int)(go->getTransform().getTranslate().x)))) % 4) - 2), 0.001f*(((abs((int)(go->getTransform().getTranslate().x) + abs((int)(go->getTransform().getTranslate().y)))) % 10) - 5)));
		//m_scene->m_gameobjects.front()->getTransform().rotate(vec3(0, 0.025f, 0));
	}
	if (keys[GLFW_KEY_ESCAPE]){
		quit();
	}
	
	
	//while (vr::VRSystem()->PollNextEvent(&ev, sizeof(ev)));
	/*(*m_scene->m_directionallights.begin())->m_direction.x = (glm::rotate(0.05f, glm::vec3(0.0f, 0.0f, 1.0f)) * vec4((*m_scene->m_directionallights.begin())->m_direction, 1.0f)).x;
	(*m_scene->m_directionallights.begin())->m_direction.y = (glm::rotate(0.05f, glm::vec3(0.0f, 0.0f, 1.0f)) * vec4((*m_scene->m_directionallights.begin())->m_direction, 1.0f)).y;
	(*m_scene->m_directionallights.begin())->m_direction.z = (glm::rotate(0.05f, glm::vec3(0.0f, 0.0f, 1.0f)) * vec4((*m_scene->m_directionallights.begin())->m_direction, 1.0f)).z;*/
}

GLvoid MainGame::render(GLdouble time)
{		
	
	//m_scene->render();
	if (m_scene != nullptr && m_scenerenderer != nullptr)
	{
		m_scenerenderer->render(m_scene, this);
		
	}
}

void MainGame::init()
{
	m_scene = new Scene(m_sizex, m_sizey);
	(m_scene)->load("..\\..\\Assets\\TestScene.txt");
	

	Billboard* b = new Billboard();
	m_scene->m_textures.back()->setWrapModeS(GL_CLAMP_TO_EDGE);
	m_scene->m_textures.back()->setWrapModeT(GL_CLAMP_TO_EDGE);
	m_scene->m_textures.back()->buffer(false);

	b->setTexture(m_scene->m_textures.back());

	b->setCamera(m_scene->m_camera);
	b->setShader(m_scene->m_shaders.back());
	//b->getTransform().setRotate(radians(-45.f), radians(-90.f), 0);
	b->getTransform().setScale(vec3(5.f, 1.f,1.f));
	b->getTransform().setTranslate(vec3(-5.f, -1.f, 1.f));
	b->setTarget(m_scene->m_renderables.find(OPAQUE)->second);
	b->lockAxis(vec3(1, 1, 0));
	m_scene->addBillboard(b);

	b = new Billboard();
	b->setTexture(m_scene->m_textures.back());
	b->getTransform().setScale(vec3(5.f, 1.f, 1.f));
	b->setCamera(m_scene->m_camera);
	b->setShader(m_scene->m_shaders.back());
	b->setTarget(m_scene->m_camera);
	m_scene->addBillboard(b);
	//m_scenerenderer = new ForwardRenderer();
	m_scenerenderer = new DeferredRenderer();
	//m_scenerenderer = new VRRenderer(new ForwardRenderer(), new Shader("..\\..\\Assets\\Shader\\quad.vert", "..\\..\\Assets\\Shader\\quad.frag"));
}

void MainGame::shutdown()
{
	//dynamic_cast<TestScene*>(m_scene)->Scene::save("Assets\\TestSceneSaveTest.txt");
	//m_scene->clear();
}
