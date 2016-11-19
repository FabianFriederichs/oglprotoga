#pragma once
#include "Scene.h"
#include "glerror.h"

//this callback handles the swapchain swap after rendering completes
class RenderFinishedCallback
{
public:
	RenderFinishedCallback(){}
	virtual void renderFinished() = 0;
};

class SceneRenderer
{
public:
	SceneRenderer();
	virtual ~SceneRenderer();

	virtual void AdjustViewport(int width, int height){};

	//make sure to call the callback via _callback.renderFinished() after rendering!
	virtual void render(Scene* _scene, RenderFinishedCallback* _callback) = 0;
	virtual void render(Scene* _scene, RenderFinishedCallback* _callback, glm::mat4* _view, glm::mat4* _proj){};
};

