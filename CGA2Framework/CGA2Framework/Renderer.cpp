#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::renderforward(RenderList& _renderlist,
	Camera* _camera,
	const std::list<DirectionalLight*>& _dirlights,
	const std::list<PointLight*>& _pointlights,
	const std::list<SpotLight*>& _spotlights)
{
	//renderstates
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	ForwardShader* curShader = nullptr;
	RenderableGameObject* curGO = nullptr;
	for (std::list<RenderPair>::const_iterator iterator = _renderlist.begin(), end = _renderlist.end(); iterator != end; ++iterator)
	{
		if ((curShader == nullptr) || curShader->getID() != iterator->first->getMaterial()->getShader()->getID())
		{
			if (curShader = dynamic_cast<ForwardShader*>(iterator->first->getMaterial()->getShader())) //ignore meshes without forward shader
			{
				curGO = nullptr;
				curShader->Use();
				curShader->setViewMatrix(_camera->GetViewMatrix());
				curShader->setProjectionMatrix(_camera->getProjectionMatrix());
				//lights
				curShader->setLights(_dirlights, _pointlights, _spotlights);
				curShader->setCameraPos(_camera->GetPosition());
			}
			else
			{
				continue;
			}
		}


		if ((curGO == nullptr) || curGO->getID() != iterator->second->getID())
		{
			curGO = iterator->second;
			curShader->setModelMatrix(curGO->getTransform().getTransformMat());
			//more uniforms 
		}

		//set material uniforms
		iterator->first->getMaterial()->setMaterialUniforms();
		curShader->preRenderActions();
		iterator->first->drawMesh();
		curShader->postRenderActions();
	}

	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
}

void Renderer::rendermultipass(RenderList& _renderlist,
	Camera* _camera,
	const std::list<DirectionalLight*>& _dirlights,
	const std::list<PointLight*>& _pointlights,
	const std::list<SpotLight*>& _spotlights)
{

}
