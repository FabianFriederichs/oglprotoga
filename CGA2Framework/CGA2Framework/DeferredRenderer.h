#pragma once
#include "SceneRenderer.h"
class DeferredRenderer :
	public SceneRenderer
{
public:
	DeferredRenderer();
	~DeferredRenderer();

	void render(Scene* _scene, RenderFinishedCallback* _callback) override;
	void render(Scene* _scene, RenderFinishedCallback* _callback, glm::mat4* _view, glm::mat4* _proj) override;

private:
	//teststuff
	bool skyboxinited = false;
	GLuint skyboxvao;
	GLuint skyboxvbo;

	Shader* skyboxshader;
	Shader* quadshader;
};

