#include "DeferredRenderer.h"



DeferredRenderer::DeferredRenderer()
{

}


DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback)
{
	if (_callback != nullptr)
		_callback->renderFinished();
}

void DeferredRenderer::render(Scene* _scene, RenderFinishedCallback* _callback, glm::mat4* _view, glm::mat4* _proj)
{
	if (_callback != nullptr)
		_callback->renderFinished();
}