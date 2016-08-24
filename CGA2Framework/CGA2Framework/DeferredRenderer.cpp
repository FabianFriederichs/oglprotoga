#include "DeferredRenderer.h"



DeferredRenderer::DeferredRenderer()
{

}


DeferredRenderer::~DeferredRenderer()
{
}

bool DeferredRenderer::init()
{
	//shaders
	quadshader = new Shader();
	quadshader->load("..\\..\\Assets\\Shader\\quadshader.vert", "..\\..\\Assets\\Shader\\quadshader.frag");

	gpassshader = new Shader();
	gpassshader->load("..\\..\\Assets\\Shader\\GPassShader.vert", "..\\..\\Assets\\Shader\\GPassShader.frag");

	skyboxshader = new Shader();
	skyboxshader->load("..\\..\\Assets\\Shader\\SkyBoxShader.vert", "..\\..\\Assets\\Shader\\SkyBoxShader.frag");

	lpassshader = new Shader();
	lpassshader->load("..\\..\\Assets\\Shader\\LPassShader.vert", "..\\..\\Assets\\Shader\\LPassShader.frag");

	//posteffect shader

	//framebuffers
	gbuffer = new FrameBuffer(0, 0, 800, 600, FBTYPE::FBT_2D);
	if (!gbuffer->allocate())
	{
		return false;
	}

	if (!gbuffer->bind(FBO_BINDINGMODE::FREADWRITE))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("position", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("normals", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("albedo", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("specular", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE))
	{
		return false;
	}

	if (!gbuffer->setDepthBufferTex(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->complete())
	{
		return false;
	}

	if (!gbuffer->setDrawBuffers())
	{
		return false;
	}

	if (!gbuffer->unbind())
	{
		return false;
	}

	inited = true;
	return true;
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback)
{
	//initial stuff setup
	if (!inited)
	{
		if (!init())
		{
			std::cout << "Error initializing renderer." << std::endl;
			return;
		}
	}

	//iterate all renderable gameobjects and render them with the gpass shader

	//bind gbuffer for drawing
	if (!gbuffer->bind(FBO_BINDINGMODE::FREADWRITE))
	{
		return;
	}

	//activate gpass shader
	gpassshader->Use();

	//per frame uniforms
	gpassshader->setUniform("view", _scene->m_camera->GetViewMatrix());
	gpassshader->setUniform("projection", _scene->m_camera->getProjectionMatrix());

	//draw all renderable gameobjects
	for (auto go : _scene->m_gameobjects)
	{
		//set per go uniforms
		gpassshader->setUniform("model", go->getTransform().getTransformMat());

		//draw go meshes
		for (auto m : go->getModel()->getMeshes())
		{

		}
	}



	//(render shadowmaps)

	//render (non alpha blended) billboards

	//render skybox

	//back to default framebuffer
	gbuffer->unbind();

	//do lighting pass


	//render transparent stuff


	//render hud

	//let buffers swap
	if (_callback != nullptr)
		_callback->renderFinished();
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback, glm::mat4* _view, glm::mat4* _proj)
{
	if (_callback != nullptr)
		_callback->renderFinished();
}