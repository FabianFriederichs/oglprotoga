#pragma once
#include "Shader.h"
class TWODShader :
	public Shader
{
public:
	TWODShader();
	virtual ~TWODShader();


	virtual void preRenderActions();
	virtual void postRenderActions();

private:
	GLboolean wasDepthEnabled;
};

