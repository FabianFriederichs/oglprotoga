#pragma once
#include "Shader.h"
#include "glerror.h"
class GPassShader :
	public Shader
{
public:
	GPassShader();
	~GPassShader();

	virtual void preRenderActions() {};
	virtual void postRenderActions() {};
};

