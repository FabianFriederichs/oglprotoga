#pragma once
#include "GameWindow.h"
#include "ForwardRenderer.h"
#include "DeferredRenderer.h"
#include "VRRenderercpp.h"
#include "Billboard.h"
#include "glerror.h"
class MainGame : public GameWindow
{
public:
	MainGame::MainGame(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver, const GLint msaasamples);
	virtual ~MainGame();

	void init();
	void shutdown();

	virtual GLvoid update(GLdouble time);
	virtual GLvoid render(GLdouble time);


	virtual void keycallback(int key, int scancode, int action, int mods);
	virtual void mmcallback(double xpos, double ypos);
	virtual void mbcallback(int button, int action, int mods);
	virtual void mscrcallback(double xoffset, double yoffset);
	bool keys[1024];

	void initVR()
	{
		if (vrR == nullptr)
		{
			auto quadshader = new Shader();
			quadshader->load("..\\..\\Assets\\Shader\\quad.vert", "..\\..\\Assets\\Shader\\quad.frag");
			vrR = new VRRenderer((m_scenerenderer!=vrR?m_scenerenderer:forward), quadshader);		
		}
		/*if (m_scenerenderer == vrR)
		{
			m_scenerenderer = vrR->getRenderer();
		}
		else
		{
			vrR->setRenderer(m_scenerenderer);
			m_scenerenderer = vrR;
		}*/
	}

private:
	Scene* m_scene;
	SceneRenderer* forward;
	SceneRenderer* def;
	VRRenderer* vrR;
	GLdouble m_curmousex;
	GLdouble m_curmousey;
};

