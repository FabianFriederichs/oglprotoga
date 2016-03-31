#pragma once
#include "GameWindow.h"
class MainGame : public GameWindow
{
public:
	MainGame(const GLint sizex, const GLint sizey, const GLboolean uselatestglver, const GLint cvmaj, const GLint cvmin, const std::string& title);		
	virtual ~MainGame();

	virtual GLvoid update(GLdouble time, GLdouble deltatime);
	virtual GLvoid render(GLdouble time, GLdouble deltatime);

	virtual void keycallback(int key, int scancode, int action, int mods);
	virtual void mmcallback(double xpos, double ypos);
	virtual void mbcallback(int button, int action, int mods);
	virtual void mscrcallback(double xoffset, double yoffset);
};

