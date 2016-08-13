#include "ForwardRenderer.h"

//GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//	// Positions   // TexCoords
//	-1.0f, 1.0f, 0.0f, 1.0f,
//	-1.0f, -1.0f, 0.0f, 0.0f,
//	1.0f, -1.0f, 1.0f, 0.0f,
//
//	-1.0f, 1.0f, 0.0f, 1.0f,
//	1.0f, -1.0f, 1.0f, 0.0f,
//	1.0f, 1.0f, 1.0f, 1.0f
//};

ForwardRenderer::ForwardRenderer()
{
	

	//
	//glGenVertexArrays(1, &testvao);
	//glGenBuffers(1, &testvbo);

	//glBindVertexArray(testvao);
	//glBindBuffer(GL_ARRAY_BUFFER,testvbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), (void*)quadVertices, GL_STATIC_DRAW);

	////vertices
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	//glBindVertexArray(0); // Unbind VAO

	////testshader
	//textestshader = new Shader();
	//textestshader->load("TextureTest.vert", "TextureTest.frag");
	//textestshader->Use();

	////testtexture
	//DDSLoader loader;
	//wood = loader.loadDDSTex("Assets\\Materials\\wooddiff.dds");

	//wood->loadGLTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	//wood->bindToTextureUnit(0);

	//textestshader->setUniform("tex", 0);		
}


ForwardRenderer::~ForwardRenderer()
{
}



void ForwardRenderer::render(Scene* _scene, RenderFinishedCallback* _callback)
{	

	glClearColor(0.15f, 0.15f, 0.18f, 1.0f); GLERR
	glEnable(GL_CULL_FACE); GLERR
	glFrontFace(GL_CCW); GLERR
	glCullFace(GL_BACK); GLERR
	glEnable(GL_DEPTH_TEST); GLERR
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR



		//textured quad test
	/*glBindVertexArray(testvao);
	wood->bindToTextureUnit(0);

	textestshader->setUniform("tex", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);*/
	
	
	for each(RenderableGameObject* g in _scene->m_gameobjects)
	{
		//prepare objects for rendering if the aren't yet
		ForwardShader* currentshader = nullptr;
		//GLint currentshaderid = -1;
		for each(Mesh* m in g->getModel()->getMeshes())
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
			}
			
			//set Material Uniforms
			material->setMaterialUniforms();			
			
			//now draw it
			currentshader->preRenderActions();
			m->drawMesh();
			currentshader->postRenderActions();
		}
	}
	glDisable(GL_DEPTH_TEST); GLERR
	_callback->renderFinished();
}
