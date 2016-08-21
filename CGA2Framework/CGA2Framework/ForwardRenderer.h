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
private:
	//teststuff
	bool skyboxinited = false;
	GLuint skyboxvao;
	GLuint skyboxvbo;

	Shader* skyboxshader;	
};

