#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::renderforward(RenderList& _renderlist,
	Camera* _camera,
	std::vector<DirectionalLight*>* _dirlights,
	std::vector<PointLight*>* _pointlights,
	std::vector<SpotLight*>* _spotlights)
{
	Shader* curShader = nullptr;
	RenderableGameObject* curGO = nullptr;
	for (std::list<RenderPair>::iterator iterator = _renderlist.begin(), end = _renderlist.end(); iterator != end; ++iterator)
	{
		if ((curShader == nullptr) || curShader->getID() != iterator->first->getShader()->getID())
		{
			curShader = iterator->first->getShader();
			curGO = nullptr;
			curShader->Use();
			curShader->setUniform("view", _camera->GetViewMatrix());
			curShader->setUniform("projection", _camera->getProjectionMatrix());
			//lights
		}


		if ((curGO == nullptr) || curGO->getID() != iterator->second->getID())
		{
			curGO = iterator->second;
			curShader->setUniform("model", curGO->getTransform().getTransformMat());
			//more uniforms 
		}


		//iterator->first->getMaterial()->fillShaderUniforms(curShader);
		curShader->preRenderActions();
		iterator->first->drawMesh();
		curShader->postRenderActions();
	}
}

void Renderer::rendermultipass(RenderList& _renderlist,
	std::vector<DirectionalLight*>* _dirlights,
	std::vector<PointLight*>* _pointlights,
	std::vector<SpotLight*>* _spotlights)
{

}
