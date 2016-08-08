#include "TWODShader.h"


TWODShader::TWODShader()
{
}


TWODShader::~TWODShader()
{
}

void TWODShader::preRenderActions()
{
	glGetBooleanv(GL_DEPTH_BUFFER_BIT, &wasDepthEnabled); GLERR
		glDisable(GL_DEPTH_TEST); GLERR
}

void TWODShader::postRenderActions()
{
	if (wasDepthEnabled)
	{
		glEnable(GL_DEPTH_TEST); GLERR
	}
}
