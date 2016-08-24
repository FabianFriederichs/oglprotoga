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

		if (_callback != nullptr)
			_callback->renderFinished();
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback, glm::mat4* _view, glm::mat4* _proj)
{
	if (!skyboxinited)
	{
		skyboxshader = new Shader();
		skyboxshader->load("..\\..\\Assets\\Shader\\SkyBox.vert", "..\\..\\Assets\\Shader\\SkyBox.frag");

		glGenVertexArrays(1, &skyboxvao);
		glGenBuffers(1, &skyboxvbo);
		glBindVertexArray(skyboxvao);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxvbo);
		glBufferData(GL_ARRAY_BUFFER, Primitives::SizeOfCubeVertices, (void*)Primitives::CubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);
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