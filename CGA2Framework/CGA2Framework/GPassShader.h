#pragma once
#include "Shader.h"
class GPassShader :
	public Shader
{
public:
	GPassShader();
	~GPassShader();

	virtual void preRenderActions();
	virtual void postRenderActions();
};

