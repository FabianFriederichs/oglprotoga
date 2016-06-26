#pragma once
#include "GameWindow.h"
#include "Scene.h"
class MainGame : public GameWindow
{
public:
	MainGame::MainGame(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver);
	virtual ~MainGame();

	void init();
	void shutdown();

	virtual GLvoid update(GLdouble time);
	virtual GLvoid render(GLdouble time);


	virtual void keycallback(int key, int scancode, int action, int mods);
	virtual void mmcallback(double xpos, double ypos);
	virtual void mbcallback(int button, int action, int mods);
	virtual void mscrcallback(double xoffset, double yoffset);

private:
	Scene* m_scene;
};

