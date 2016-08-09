#include "GameWindow.h"


GameWindow::GameWindow(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver)
{
	this->m_sizex = sizex;
	this->m_sizey = sizey;
	this->m_uselatestglcontext = uselatestglver;
	this->m_cvmaj = cvmaj;
	this->m_cvmin = cvmin;
	this->m_title = title;
	if (!this->initialize())
		throw "Initiatization error: ";
}


GameWindow::~GameWindow()
{
	glfwTerminate();
}

GLboolean GameWindow::initialize()
{
	//glfwSetErrorCallback({})

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->m_cvmaj);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->m_cvmin);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	this->m_window = glfwCreateWindow(m_sizex, m_sizey, m_title.c_str(), NULL, NULL);
	if (this->m_window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(this->m_window);

	glewExperimental = GL_TRUE;
	// Initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		getchar();
		glfwTerminate();
		return false;
	}

	//std::cout << glewGetString(GL_EXTENSIONS) << std::endl;

	err = glGetError();
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glfwSetInputMode(this->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(this->m_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(this->m_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
	
	glfwSetKeyCallback(m_window, GLFWHandler::key_dispatch);
	glfwSetCursorPosCallback(m_window, GLFWHandler::mm_dispatch);
	glfwSetMouseButtonCallback(m_window, GLFWHandler::mb_dispatch);
	glfwSetScrollCallback(m_window, GLFWHandler::mscr_dispatch);

	setHandlerInstance();
	return true;
}

GLvoid GameWindow::setCursorVisble(GLboolean visible)
{
	if (this->m_window != NULL)
	{
		glfwSetInputMode(this->m_window, GLFW_CURSOR, (visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED));
	}
}

void GameWindow::keycallback( int key, int scancode, int action, int mods)
{
}

void GameWindow::mmcallback(double xpos, double ypos)
{

}
void GameWindow::mbcallback(int button, int action, int mods)
{

}

void GameWindow::mscrcallback(double xoffset, double yoffset)
{

}

GLvoid GameWindow::run()
{
	init();
	GLdouble timeDelta = 1.0f / 60.0f;
	GLdouble timeAccumulator = 0;
	GLdouble startTime;
	while (!glfwWindowShouldClose(this->m_window))
	{
		startTime = glfwGetTime();
		glfwPollEvents();
		while (timeAccumulator >= timeDelta)
		{
			update(timeAccumulator);
			timeAccumulator -= timeDelta;
		}
		render(timeAccumulator);
		timeAccumulator += glfwGetTime() - startTime;
	}
	shutdown();
	glfwTerminate();	
	delete m_window;
}

GLvoid GameWindow::quit()
{
	glfwSetWindowShouldClose(m_window, GL_TRUE);
}
