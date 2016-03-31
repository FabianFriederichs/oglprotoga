#include "MainGame.h"
#include "headers.h"


MainGame::MainGame(const GLint sizex, const GLint sizey, const GLboolean uselatestglver, const GLint cvmaj, const GLint cvmin, const std::string& title) 
	: GameWindow(sizex, sizey, uselatestglver, cvmaj, cvmin, title)
{
}


MainGame::~MainGame()
{
}

void MainGame::keycallback(int key, int scancode, int action, int mods)
{

}

void MainGame::mmcallback(double xpos, double ypos)
{
	fprintf(stderr, "Mousepos: X: %d Y: %d", xpos, ypos);
}
void MainGame::mbcallback(int button, int action, int mods)
{

}

void MainGame::mscrcallback(double xoffset, double yoffset)
{

}

GLvoid MainGame::update(GLdouble time, GLdouble deltatime)
{

}

GLvoid MainGame::render(GLdouble time, GLdouble deltatime)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(this->m_window);
}
