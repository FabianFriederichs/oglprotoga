#include "ForwardShader.h"


ForwardShader::ForwardShader()
{
}


ForwardShader::~ForwardShader()
{
}

void ForwardShader::preRenderActions()
{
	glEnable(GL_DEPTH_TEST);
}

void ForwardShader::postRenderActions()
{
	glDisable(GL_DEPTH_TEST);
}