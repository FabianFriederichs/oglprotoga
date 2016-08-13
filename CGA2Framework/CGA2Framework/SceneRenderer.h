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

	//make sure to call the callback via _callback.renderFinished() after rendering!
	virtual void render(Scene* _scene, RenderFinishedCallback* _callback) = 0;
};
