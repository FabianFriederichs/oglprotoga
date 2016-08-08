#include "ForwardRenderer.h"


ForwardRenderer::ForwardRenderer()
{
	glClearColor(0.2f, 0.2f, 1.0f, 1.0f); GLERR
		//glClearDepth(0.0f); GLERR
	glEnable(GL_CULL_FACE); GLERR
	glFrontFace(GL_CCW); GLERR
	glEnable(GL_DEPTH_TEST); GLERR
}


ForwardRenderer::~ForwardRenderer()
{
}

void ForwardRenderer::render(Scene* _scene, RenderFinishedCallback* _callback)
{
	std::cout << "Render now" << std::endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR
	
	
	
	for each(RenderableGameObject* g in _scene->m_gameobjects)
	{
		//prepare objects for rendering if the aren't yet
		GLint currentshaderid = -1;
		for each(Mesh* m in g->getModel()->getMeshes())
		{
			if (!m->hasNormals())
				m->generateNormals();
			if (!m->hasTangents())
				m->generateTangents();			
			m->setupVAOs();

			Material* material = m->getMaterial();
			ForwardShader* shader = dynamic_cast<ForwardShader*>(m->getMaterial()->getShader());
			
				shader->Use();
				//set Material Uniforms
				material->setMaterialUniforms();

				//set Lights
				shader->setLights(_scene->m_directionallights, _scene->m_pointlights, _scene->m_spotlights);

				//set view and projection matrix
				shader->setViewMatrix(_scene->m_camera->GetViewMatrix());
				shader->setProjectionMatrix(_scene->m_camera->getProjectionMatrix());
				shader->setCameraPos(_scene->m_camera->GetPosition());
			

			//set model matrix
			shader->setModelMatrix(g->getTransform().getTransformMat());
			
			//now draw it
			shader->preRenderActions();
			m->drawMesh();
			shader->postRenderActions();			
		}
	}
	_callback->renderFinished();
}
