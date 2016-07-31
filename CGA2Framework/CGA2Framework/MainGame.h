#pragma once
#include "GameWindow.h"
#include "FPSCamera.h"
#include "Shader.h"
class MainGame : public GameWindow
{

	const static GLfloat vertices[];

public:
	MainGame::MainGame(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver);
	virtual ~MainGame();

	virtual GLvoid update(GLdouble time);
	virtual GLvoid render(GLdouble time);

	virtual void keycallback(int key, int scancode, int action, int mods);
	virtual void mmcallback(double xpos, double ypos);
	virtual void mbcallback(int button, int action, int mods);
	virtual void mscrcallback(double xoffset, double yoffset);

	quat RotateQuat(const vec3 &rotation);

	bool keys[1024];

	GLuint VBO, VAO;
	Camera *cam;
	Shader *shader;

	GLfloat m_VA;
	GLfloat m_HA;
};

