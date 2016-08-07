#pragma once
#include "headers.h"
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	GLuint m_fbuffer;
	GLint m_vpoffx;
	GLint m_vpoffy;
	GLint m_vpwidth;
	GLint m_vpheight;

	bool bind() { return true; }

	bool setColorTargets(std::vector<Texture*> targetTextures);
	bool setDepthTarget(Texture* textureTarget);
};

