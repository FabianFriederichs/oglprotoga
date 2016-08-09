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
	glClearColor(0.2f, 0.2f, 1.0f, 1.0f); GLERR
		//glClearDepth(0.0f); GLERR
	glEnable(GL_CULL_FACE); GLERR
	glFrontFace(GL_CCW); GLERR
	glEnable(GL_DEPTH_TEST); GLERR

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
	//std::cout << "Render now" << std::endl;
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
