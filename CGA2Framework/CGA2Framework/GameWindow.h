#pragma once
#include "headers.h"
#include "GLFWHandler.h"
#include "SceneRenderer.h"
#include "glerror.h"



class GameWindow : public GLFWHandler, public RenderFinishedCallback
{
public:
	GameWindow(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver);
	virtual ~GameWindow();
	//initialize glew and glfw window
	
	GLvoid setCursorVisble(GLboolean visible);
	//gameloop
	GLvoid run();
	GLvoid quit();

	//getters/setters
	SceneRenderer* getSceneRenderer() { return m_scenerenderer; }
	void setSceneRenderer(SceneRenderer* _renderer) { this->m_scenerenderer = _renderer; }


	virtual GLvoid update(GLdouble time) = 0;
	virtual GLvoid render(GLdouble time) = 0;
	virtual GLvoid init() = 0;
	virtual GLvoid shutdown() = 0;

	virtual GLvoid renderFinished() 
	{
		if (m_window != nullptr)
		{
			glfwSwapBuffers(m_window);
		}
	}
protected:
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

	//Scene renderer
	//Call m_scenerederer.render(&somescene, this) when it's rendertime!
	SceneRenderer* m_scenerenderer;

protected:
	GLFWwindow* m_window;
};

