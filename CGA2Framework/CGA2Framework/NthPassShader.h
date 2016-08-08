#pragma once
#include "Shader.h"
#include "glerror.h"
class NthPassShader :
	public Shader
{
public:
	NthPassShader();
	virtual ~NthPassShader();

	//getters / setters
	GLint getPass() { return m_pass; }
	void setPass(GLint pass) { m_pass = pass; }

	//pre and post actions
	virtual void preRenderActions(){};
	virtual void postRenderActions(){};

private:
	GLint m_pass; //Specifies the pass of the Shader, starting with 1. The first pass is always a GPassShader
};

