#pragma once
#include "Scene.h"
#include "FPSCamera.h"
#include "glerror.h"
class TestScene :
	public Scene
{
public:
	TestScene(GLint width, GLint height);
	~TestScene();

	void save()override{};
	void load()override;
};