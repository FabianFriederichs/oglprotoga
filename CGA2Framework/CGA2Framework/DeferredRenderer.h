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
	bool inited = false;

	Shader* skyboxshader;
	Shader* quadshader;

	//later
	Shader* gpassshader;	//Position, Normals, Albedo and Specular Rendertargets + Depth Renderbuffer
	Shader* lightingpassshader;

	FrameBuffer* gbuffer;
};

