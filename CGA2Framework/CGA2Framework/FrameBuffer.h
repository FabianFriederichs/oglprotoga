#pragma once
#include "headers.h"
#include "Texture.h"
#include "glerror.h"
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	GLuint m_fbuffer;
	GLint m_vpwidth;
	GLint m_vpheight;

	bool allocate() { return true; }
	bool bind() { return true; }

	bool setColorTargets(std::vector<Texture*> targetTextures);
	bool setDepthTarget(Texture* textureTarget);
	//ignore packed depth/stencil buffer for now

};

