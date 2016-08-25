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
	quadshader->load("..\\..\\Assets\\Shader\\quad.vert", "..\\..\\Assets\\Shader\\quad.frag");

	gpassshader = new Shader();
	gpassshader->load("..\\..\\Assets\\Shader\\GPassShader.vert", "..\\..\\Assets\\Shader\\GPassShader.frag");

	skyboxshader = new Shader();
	skyboxshader->load("..\\..\\Assets\\Shader\\SkyBox.vert", "..\\..\\Assets\\Shader\\SkyBox.frag");

	/*lpassshader = new Shader();
	lpassshader->load("..\\..\\Assets\\Shader\\LPassShader.vert", "..\\..\\Assets\\Shader\\LPassShader.frag");*/

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

	if (!gbuffer->addColorBufferTex("position", GL_RGBA16F, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("normals", GL_RGBA16F, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("albedo", GL_RGBA16F, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("specular", GL_RGBA16F, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("gloss", GL_RGBA16F, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("height", GL_RGBA16F, GL_RGBA, GL_FLOAT))
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

	gbuffer->updateGLViewport(0, 0, 800, 600);

	glClearColor(1.0f, 0.15f, 0.18f, 1.0f); GLERR
	glEnable(GL_CULL_FACE); GLERR
	glFrontFace(GL_CCW); GLERR
	glCullFace(GL_BACK); GLERR
	glEnable(GL_DEPTH_TEST); GLERR
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR

	//activate gpass shader
	gpassshader->Use();

	//per frame uniforms
	gpassshader->setUniform("view", _scene->m_camera->GetCameraTransform(), false);
	gpassshader->setUniform("projection", _scene->m_camera->getProjectionMatrix(), false);
	gpassshader->setUniform("camerapos", _scene->m_camera->GetPosition());

	//draw opaque geometry
	auto it = _scene->m_renderables.equal_range(OPAQUE);
	for (auto k = it.first; k != it.second; k++)
	{
		RenderableGameObject* go = k->second;
		//set per go uniforms
		gpassshader->setUniform("model", go->getTransform().getTransformMat(), false);

		//draw go meshes
		for (auto m : go->getModel()->getMeshes())
		{
			m->getMaterial()->setMaterialUniforms(gpassshader);
			
			if (!m->hasNormals())
				m->generateNormals();
			if (!m->hasTangents())
				m->generateTangents();
			m->setupVAOs();

			m->drawMesh();
		}
	}



	glDisable(GL_DEPTH_TEST); GLERR

	//(render shadowmaps)

	//render (non alpha blended) billboards

	//render skybox

	//back to default framebuffer
	gbuffer->unbind();

	glViewport(0, 0, 800, 600);

	quadshader->Use();
	gbuffer->getColorBufferTex("position")->bindToTextureUnit(0);
	gbuffer->getColorBufferTex("normals")->bindToTextureUnit(1);
	gbuffer->getColorBufferTex("albedo")->bindToTextureUnit(2);
	gbuffer->getColorBufferTex("specular")->bindToTextureUnit(3);
	gbuffer->getColorBufferTex("gloss")->bindToTextureUnit(4);
	gbuffer->getColorBufferTex("height")->bindToTextureUnit(5);
	gbuffer->getDepthBufferTex()->bindToTextureUnit(6);
	quadshader->setUniform("screenTexture", 0);
	
	Primitives::drawNDCQuad();



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