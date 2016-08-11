#pragma once
#include "headers.h"
#include "Texture.h"
#include "glerror.h"
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	bool allocate() { return true; }
	bool destroy() { return true; }
	bool bind() { return true; }
	bool updateViewport() { return true; }

	//getters / setters
	GLuint getGLFBO() { return m_fbo; }
	GLint getViewportWidth() { return m_vpwidth; }
	GLint getViewportHeight() { return m_vpheight; }

	

private:
	//the fbo hande
	GLuint m_fbo;
	//when not using a buffer for a texture, keep handles to the renderbuffers
	std::unordered_map<std::string, GLuint> m_colorrenderbuffers;
	GLuint m_depthrenderbuffer;	
	//the actual textures
	std::unordered_map<std::string, Texture*> m_colorbuffers;
	Texture* m_depthbuffer;

	//viewport
	GLint m_vpwidth;
	GLint m_vpheight;
};

