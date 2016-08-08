#pragma once
#include "Scene.h"
class TestScene :
	public Scene
{
public:
	TestScene(GLint width, GLint height);
	~TestScene();

	void save();
	void load();
};

