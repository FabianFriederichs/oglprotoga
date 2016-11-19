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

	lpassshader = new Shader();
	lpassshader->load("..\\..\\Assets\\Shader\\LPassShader.vert", "..\\..\\Assets\\Shader\\LPassShader.frag");

	spassshader = new Shader();
	spassshader->load("..\\..\\Assets\\Shader\\ShadowShader.vert", "..\\..\\Assets\\Shader\\ShadowShader.frag");

	//posteffect shader

	GLint m_viewport[4];

	glGetIntegerv(GL_VIEWPORT, m_viewport);
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

	if (!gbuffer->addColorBufferTex("albedo", GL_RGBA8, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("specular", GL_RGBA8, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("gloss", GL_RGBA8, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->addColorBufferTex("height", GL_RGBA8, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!gbuffer->setDepthBufferTex(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT))
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

	//framebuffers
	sbuffer = new FrameBuffer(0, 0, 4000, 4000, FBTYPE::FBT_2D);
	if (!sbuffer->allocate())
	{
		return false;
	}

	if (!sbuffer->bind(FBO_BINDINGMODE::FREADWRITE))
	{
		return false;
	}
	sbuffer->updateGLViewport();// (0, 0, 800, 600);

	if (!sbuffer->addColorBufferTex("junk", GL_RGBA16F, GL_RGBA, GL_FLOAT))
	{
		return false;
	}

	if (!sbuffer->setDepthBufferTex(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT))
	{
		return false;
	}

	if (!sbuffer->complete())
	{
		return false;
	}

	if (!sbuffer->setDrawBuffers())
	{
		return false;
	}

	if (!sbuffer->unbind())
	{
		return false;
	}

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	inited = true;

	return true;
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback)
{
	render(_scene, _callback, &_scene->m_camera->GetViewMatrix(), &_scene->m_camera->getProjectionMatrix());
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback, glm::mat4* _view, glm::mat4* _proj)
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
	GLint targetBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &targetBuffer);
	GLint m_viewport[4];
	
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	//iterate all renderable gameobjects and render them with the gpass shader

	//bind gbuffer for drawing
	if (!gbuffer->bind(FBO_BINDINGMODE::FREADWRITE))
	{
		return;
	}
	
	
	gbuffer->updateGLViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GLERR

		glEnable(GL_CULL_FACE); GLERR
		glFrontFace(GL_CCW); GLERR
		glCullFace(GL_BACK); GLERR
		glEnable(GL_DEPTH_TEST); GLERR
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR

		//activate gpass shader
		gpassshader->Use();

	//per frame uniforms
	gpassshader->setUniform("view", *_view, false);
	gpassshader->setUniform("projection", *_proj, false);
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
	gbuffer->unbind();


	if (!sbuffer->bind(FBO_BINDINGMODE::FREADWRITE))
	{
		return;
	}
	sbuffer->updateGLViewport();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GLERR
		glEnable(GL_CULL_FACE); GLERR
		glFrontFace(GL_CCW); GLERR
		glCullFace(GL_BACK); GLERR
		glEnable(GL_DEPTH_TEST); GLERR
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR;
	spassshader->Use();

	auto look = glm::lookAt(vec3(-30, 30, -10), vec3(0, 0, 0), vec3(0, 1, 0));// _scene->m_directionallights.front()->
	auto ligmat = _scene->m_directionallights.front()->getProj(0.1f, 100.f)*look;
	spassshader->setUniform("ligmat", ligmat, false);
	it = _scene->m_renderables.equal_range(OPAQUE);
	for (auto k = it.first; k != it.second; k++)
	{
		RenderableGameObject* go = k->second;
		//set per go uniforms
		spassshader->setUniform("model", go->getTransform().getTransformMat(), false);

		//draw go meshes
		for (auto m : go->getModel()->getMeshes())
		{
			//m->getMaterial()->setMaterialUniforms(gpassshader);
			m->getMaterial()->getTextures().front()->bindToTextureUnit(0);
			spassshader->setUniform("alph", 0);
			if (!m->hasNormals())
				m->generateNormals();
			if (!m->hasTangents())
				m->generateTangents();
			m->setupVAOs();

			m->drawMesh();
		}
	}
	sbuffer->unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, targetBuffer);

	
	//(render shadowmaps)

	//render (non alpha blended) billboards

	//render skybox

	//back to default framebuffer
	glDisable(GL_CULL_FACE); GLERR


	glEnable(GL_DEPTH_TEST); GLERR
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	lpassshader->Use();
	gbuffer->getColorBufferTex("position")->bindToTextureUnit(0);
	gbuffer->getColorBufferTex("normals")->bindToTextureUnit(1);
	gbuffer->getColorBufferTex("albedo")->bindToTextureUnit(2);
	gbuffer->getColorBufferTex("specular")->bindToTextureUnit(3);
	gbuffer->getColorBufferTex("gloss")->bindToTextureUnit(4);
	gbuffer->getColorBufferTex("height")->bindToTextureUnit(5);
	gbuffer->getDepthBufferTex()->bindToTextureUnit(6);
	sbuffer->getDepthBufferTex()->bindToTextureUnit(7);

	lpassshader->setUniform("position", 0);
	lpassshader->setUniform("normal", 1);
	lpassshader->setUniform("albedo", 2);
	lpassshader->setUniform("specular", 3);
	lpassshader->setUniform("gloss", 4);
	lpassshader->setUniform("height", 5);
	lpassshader->setUniform("depth", 6);
	lpassshader->setUniform("shadow", 7);

	lpassshader->setUniform("camerapos", vec3(-2, 4, -2));// _scene->m_camera->GetPosition());
	lpassshader->setUniform("ligmat", ligmat, false);
	lpassshader->setUniform("pcf", pcf);
	lpassshader->setUniform("isshad", isshad);
	setLights(_scene->m_directionallights, _scene->m_pointlights, _scene->m_spotlights, lpassshader);

	glDisable(GL_DEPTH_TEST);
	Primitives::drawNDCQuad();



	//do lighting pass


	//render transparent stuff


	//render hud

	//let buffers swap
	if (_callback != nullptr)
		_callback->renderFinished();
}

void DeferredRenderer::AdjustViewport(int width, int height)
{
	if (gbuffer->getViewportWidth() != width && gbuffer->getViewportHeight() != height)
	{
		auto bufs = gbuffer->getColorBufferTexs();
		for (auto t : *bufs)
		{
			if (t.second.tex->getType() == TEXTYPE::TEX_2D)
			{
				auto t2d = dynamic_cast<Texture2D*>(t.second.tex);
				t2d->setHeight(height);
				t2d->setWidth(width);
				t2d->ReAllocate();
			}
		}
		auto depth = gbuffer->getDepthBufferTex();
		auto d = dynamic_cast<Texture2D*>(depth);
		d->setHeight(height);
		d->setWidth(width);
		d->ReAllocate();

	}
}

void DeferredRenderer::setLights(
	const std::list<DirectionalLight*>& dirLights,
	const std::list<PointLight*>& pointLights,
	const std::list<SpotLight*>& spotLights,
	Shader* _shader)
{
	GLint ct = 0;
	//set directional light uniforms
	for (std::list<DirectionalLight*>::const_iterator it = dirLights.begin(); it != dirLights.end(); it++)
	{
		if (ct + 1 > MAX_DIR_LIGHTS)
			break;
		_shader->setUniform("dirlights[" + std::to_string(ct) + "].lightdir", (*it)->m_direction);
		_shader->setUniform("dirlights[" + std::to_string(ct) + "].lightcol", (*it)->m_lightcolor);
		ct++;
	}
	_shader->setUniform("dirlightcount", ct);

	ct = 0;
	//set pointlight uniforms
	for (std::list<PointLight*>::const_iterator it = pointLights.begin(); it != pointLights.end(); it++)
	{
		if (ct + 1 > MAX_DIR_LIGHTS)
			break;
		_shader->setUniform("pointlights[" + std::to_string(ct) + "].lightpos", (*it)->getTransform().getTranslate());
		_shader->setUniform("pointlights[" + std::to_string(ct) + "].lightcol", (*it)->m_lightcolor);
		_shader->setUniform("pointlights[" + std::to_string(ct) + "].cterm", (*it)->m_constant);
		_shader->setUniform("pointlights[" + std::to_string(ct) + "].lterm", (*it)->m_linear);
		_shader->setUniform("pointlights[" + std::to_string(ct) + "].qterm", (*it)->m_quadratic);
		_shader->setUniform("pointlights[" + std::to_string(ct) + "].range", (*it)->m_range);
		ct++;
	}
	_shader->setUniform("pointlightcount", ct);

	ct = 0;
	//set spotlight uniforms
	for (std::list<SpotLight*>::const_iterator it = spotLights.begin(); it != spotLights.end(); it++)
	{
		if (ct + 1 > MAX_DIR_LIGHTS)
			break;
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].lightpos", (*it)->getTransform().getTranslate());
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].lightcol", (*it)->m_lightcolor);
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].lightdir", (*it)->getTransform().getForw());
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].cterm", (*it)->m_constant);
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].lterm", (*it)->m_linear);
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].qterm", (*it)->m_quadratic);
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].innercone", (*it)->m_cosinnercone);
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].qterm", (*it)->m_cosoutercone);
		_shader->setUniform("spotlights[" + std::to_string(ct) + "].range", (*it)->m_range);
		ct++;
	}
	_shader->setUniform("spotlightcount", ct);

}