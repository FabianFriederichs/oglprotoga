#include "MainGame.h"
#include "headers.h"


MainGame::MainGame(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver)
	: GameWindow(sizex, sizey, cvmaj, cvmin, title, uselatestglver)
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
	fprintf(stderr, "Mousepos: X: %.0f Y: %.0f\n", xpos, ypos);
}
void MainGame::mbcallback(int button, int action, int mods)
{

}

void MainGame::mscrcallback(double xoffset, double yoffset)
{

}

GLvoid MainGame::update(GLdouble time)
{

}

GLvoid MainGame::render(GLdouble time)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(this->m_window);
}
