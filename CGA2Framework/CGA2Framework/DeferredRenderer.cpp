#include "DeferredRenderer.h"



DeferredRenderer::DeferredRenderer()
{

}


DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback)
{
	if (!skyboxinited)
	{
		skyboxshader = new Shader();
		skyboxshader->load("..\\..\\Assets\\Shader\\SkyBox.vert", "..\\..\\Assets\\Shader\\SkyBox.frag");

		quadshader = new Shader();
		quadshader->load("..\\..\\Assets\\Shader\\TextureTest.vert", "..\\..\\Assets\\Shader\\TextureTest.frag");

		//create gbuffer fbo

		gbuffer = new FrameBuffer(0, 0, 800, 600, FBTYPE::FBT_2D);
		gbuffer->allocate();
		gbuffer->bind((FBO_BINDINGMODE::FREADWRITE));

		gbuffer->addColorBufferTex("position", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
		gbuffer->addColorBufferTex("normals", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
		gbuffer->addColorBufferTex("albedo", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
		gbuffer->addColorBufferTex("specular", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
		gbuffer->setDepthBufferTex(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);


		gbuffer->complete();
		gbuffer->setDrawBuffers();
		gbuffer->unbind();

		skyboxinited = true;
	}

	gbuffer->bind(FBO_BINDINGMODE::FREADWRITE);

	glClearColor(0.15f, 0.15f, 0.18f, 1.0f); GLERR
		glEnable(GL_CULL_FACE); GLERR
		glFrontFace(GL_CCW); GLERR
		glCullFace(GL_BACK); GLERR
		glEnable(GL_DEPTH_TEST); GLERR
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR

		for (RenderableGameObject* g : _scene->m_gameobjects)
		{
			//prepare objects for rendering if the aren't yet
			ForwardShader* currentshader = nullptr;
			//GLint currentshaderid = -1;
			for (Mesh* m : g->getModel()->getMeshes())
			{
				if (!m->hasNormals())
					m->generateNormals();
				if (!m->hasTangents())
					m->generateTangents();
				m->setupVAOs();

				Material* material = m->getMaterial();
				//currentshader = dynamic_cast<ForwardShader*>(m->getMaterial()->getShader());
				if (currentshader == nullptr || currentshader->getID() != m->getMaterial()->getID())
				{
					currentshader = dynamic_cast<ForwardShader*>(m->getMaterial()->getShader());
					currentshader->Use();

					//These uniforms must only be set once per shader
					//set Lights
					currentshader->setLights(_scene->m_directionallights, _scene->m_pointlights, _scene->m_spotlights);

					//set model matrix
					currentshader->setModelMatrix(g->getTransform().getTransformMat());
					//set view and projection matrix
					currentshader->setViewMatrix(_scene->m_camera->GetViewMatrix());
					currentshader->setProjectionMatrix(_scene->m_camera->getProjectionMatrix());
					currentshader->setCameraPos(_scene->m_camera->GetPosition());

					if (_scene->m_cubemaps.size() > 0)
					{
						if (!(*_scene->m_cubemaps.begin())->isBuffered())
							(*_scene->m_cubemaps.begin())->buffer(false);
						(*_scene->m_cubemaps.begin())->bindToTextureUnit(10);
					}

					currentshader->setUniform("skybox", 10);
				}

				//set Material Uniforms
				material->setMaterialUniforms();

				//now draw it
				currentshader->preRenderActions();
				m->drawMesh();
				currentshader->postRenderActions();
			}


		}
	for (auto b : _scene->m_billboards)
	{
		b->Draw();
	}
	//render the skybox



	if (_scene->m_cubemaps.size() > 0)
	{
		if (!(*_scene->m_cubemaps.begin())->isBuffered())
			(*_scene->m_cubemaps.begin())->buffer(false);
		(*_scene->m_cubemaps.begin())->bindToTextureUnit(10);
	}
	glDepthFunc(GL_LEQUAL);
	skyboxshader->Use();
	skyboxshader->setUniform("skybox", 10);
	skyboxshader->setUniform("view", glm::mat4(glm::mat3(_scene->m_camera->GetViewMatrix())), false);
	skyboxshader->setUniform("projection", _scene->m_camera->getProjectionMatrix(), false);

	Primitives::drawNDCCube();

	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST); GLERR

	gbuffer->unbind();

	//default buffer is bound again
	//FrameBuffer::blit(gbuffer, nullptr, true, 0, true, false, 0, 0, 800, 600, 0, 0, 800, 600, GL_BACK);

	quadshader->Use();
	Texture* coltex = gbuffer->getDepthBufferTex();
	coltex->bindToTextureUnit(0);
	quadshader->setUniform("tex", 0);

	Primitives::drawNDCQuad();



	if (_callback != nullptr)
		_callback->renderFinished();
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback, glm::mat4* _view, glm::mat4* _proj)
{
	if (!skyboxinited)
	{
		skyboxshader = new Shader();
		skyboxshader->load("..\\..\\Assets\\Shader\\SkyBox.vert", "..\\..\\Assets\\Shader\\SkyBox.frag");
		//create gbuffer fbo

		gbuffer = new FrameBuffer(0, 0, 800, 600, FBTYPE::FBT_2D);
		gbuffer->allocate();
		gbuffer->bind((FBO_BINDINGMODE::FREADWRITE));

		gbuffer->addColorBufferTex("color", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
		gbuffer->setDepthBufferTex(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);
		
		gbuffer->complete();
		gbuffer->setDrawBuffers();
		gbuffer->unbind();

		skyboxinited = true;
	}


	glClearColor(0.15f, 0.15f, 0.18f, 1.0f); GLERR
		glEnable(GL_CULL_FACE); GLERR
		glFrontFace(GL_CCW); GLERR
		glCullFace(GL_BACK); GLERR
		glEnable(GL_DEPTH_TEST); GLERR
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR

		for (RenderableGameObject* g : _scene->m_gameobjects)
		{
			//prepare objects for rendering if the aren't yet
			ForwardShader* currentshader = nullptr;
			//GLint currentshaderid = -1;
			for (Mesh* m : g->getModel()->getMeshes())
			{
				if (!m->hasNormals())
					m->generateNormals();
				if (!m->hasTangents())
					m->generateTangents();
				m->setupVAOs();

				Material* material = m->getMaterial();
				//currentshader = dynamic_cast<ForwardShader*>(m->getMaterial()->getShader());
				if (currentshader == nullptr || currentshader->getID() != m->getMaterial()->getID())
				{
					currentshader = dynamic_cast<ForwardShader*>(m->getMaterial()->getShader());
					currentshader->Use();

					//These uniforms must only be set once per shader
					//set Lights
					currentshader->setLights(_scene->m_directionallights, _scene->m_pointlights, _scene->m_spotlights);

					//set model matrix
					currentshader->setModelMatrix(g->getTransform().getTransformMat());
					//set view and projection matrix
					currentshader->setViewMatrix(*_view);
					currentshader->setProjectionMatrix(*_proj);
					currentshader->setCameraPos(_scene->m_camera->GetPosition());

					if (_scene->m_cubemaps.size() > 0)
					{
						if (!(*_scene->m_cubemaps.begin())->isBuffered())
							(*_scene->m_cubemaps.begin())->buffer(false);
						(*_scene->m_cubemaps.begin())->bindToTextureUnit(10);
					}

					currentshader->setUniform("skybox", 10);
				}

				//set Material Uniforms
				material->setMaterialUniforms();

				//now draw it
				currentshader->preRenderActions();
				m->drawMesh();
				currentshader->postRenderActions();
			}
		}

	//render the skybox

	if (_scene->m_cubemaps.size() > 0)
	{
		if (!(*_scene->m_cubemaps.begin())->isBuffered())
			(*_scene->m_cubemaps.begin())->buffer(false);
		(*_scene->m_cubemaps.begin())->bindToTextureUnit(10);
	}
	glDepthFunc(GL_LEQUAL);
	skyboxshader->Use();
	skyboxshader->setUniform("skybox", 10);
	skyboxshader->setUniform("view", glm::mat4(glm::mat3(*_view)), false);
	skyboxshader->setUniform("projection", *_proj, false);

	Primitives::drawNDCCube();

	glDepthFunc(GL_LESS);

	glDisable(GL_DEPTH_TEST); GLERR
		if (_callback != nullptr)
			_callback->renderFinished();
}