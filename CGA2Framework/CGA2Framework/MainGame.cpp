#include "MainGame.h"
#include "headers.h"


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
			m_scene->m_camera->Move(md);
	}
	if (keys[GLFW_KEY_ESCAPE]){
		quit();
	}
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
	m_scene = new TestScene(m_sizex, m_sizey);
	dynamic_cast<TestScene*>(m_scene)->Scene::load("Assets\\TestScene.txt");
	m_scenerenderer = new ForwardRenderer();
}

void MainGame::shutdown()
{
	dynamic_cast<TestScene*>(m_scene)->Scene::save("Assets\\TestSceneSaveTest.txt");
	//m_scene->clear();
}
