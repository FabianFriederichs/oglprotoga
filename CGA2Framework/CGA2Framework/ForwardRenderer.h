#pragma once
#include "SceneRenderer.h"
#include "Primitives.h"
#include "glerror.h"
class ForwardRenderer :
	public SceneRenderer
{
public:
	ForwardRenderer();
	~ForwardRenderer();

	void render(Scene* _scene, RenderFinishedCallback* _callback) override;
	void render(Scene* _scene, RenderFinishedCallback* _callback, glm::mat4* _view, glm::mat4* _proj) override;
	void setRef(int i){ refract = i; }
	void setRefl(int i){ reflect = i; }
	GLint refract=0;
	GLint reflect=0;
private:
	//teststuff
	bool skyboxinited = false;
	GLuint skyboxvao;
	GLuint skyboxvbo;

	Shader* skyboxshader;
	Shader* quadshader;
};

