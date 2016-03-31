#pragma once
#include "headers.h"
#include "GLFWHandler.h"



class GameWindow : public GLFWHandler
{
public:
	GameWindow(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver);
	virtual ~GameWindow();
	//initialize glew and glfw window
	
	GLvoid setCursorVisble(GLboolean visible);
	//gameloop
	GLvoid run();
	GLvoid quit();


	virtual GLvoid update(GLdouble time) = 0;
	virtual GLvoid render(GLdouble time) = 0;
private:
	GLint m_sizex;
	GLint m_sizey;
	std::string m_title;

	//glfw callbacks
	virtual void keycallback(int key, int scancode, int action, int mods);
	virtual void mmcallback(double xpos, double ypos);
	virtual void mbcallback(int button, int action, int mods);
	virtual void mscrcallback(double xoffset, double yoffset);

	//context config
	GLboolean initialize();
	GLboolean m_uselatestglcontext;
	GLint m_cvmaj;
	GLint m_cvmin;

protected:
	GLFWwindow* m_window;
};

