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
	void AdjustViewport(int width, int height) override;
	void setPCF(int i){ pcf = i; }
	void setShad(int i){ isshad = i; }
	GLint pcf = 0;
	GLint isshad = 0;
private:
	//init
	bool inited = false;
	bool init();

	//shaders
	Shader* skyboxshader;
	Shader* quadshader;
	Shader* gpassshader;	//Position, Normals, Albedo and Specular Rendertargets + Depth Renderbuffer
	Shader* lpassshader;
	Shader* spassshader;

	//framebuffers
	FrameBuffer* gbuffer;
	FrameBuffer* sbuffer;

	void setLights(const std::list<DirectionalLight*>& dirlights, const std::list<PointLight*>& pointlights, const std::list<SpotLight*>& spotlights, Shader* _shader);
};

