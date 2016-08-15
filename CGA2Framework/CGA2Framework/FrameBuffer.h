#pragma once
#include "headers.h"
#include "Texture.h"
#include "glerror.h"

typedef enum {
	FREAD,
	FWRITE,
	FREADWRITE
} FBO_BINDINGMODE;

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(const GLint _vpxoff, const GLint _vpyoff, const GLint _vpwidth, const GLint _vpheight);
	~FrameBuffer();

	bool allocate();
	bool destroy();
	bool bind(const FBO_BINDINGMODE _bindingmode);
	bool unbind();
	bool updateGLViewport();
	bool updateGLViewport(const GLint _vpxoff, const GLint _vpyoff, const GLint _vpwidth, const GLint _vpheight);
	bool blit(FrameBuffer* _target);

	//getters / setters
	GLuint getGLFBO() { return m_fbo; }
	GLint getViewportWidth() { return m_vpwidth; }
	GLint getViewportHeight() { return m_vpheight; }

	//access the textures
	Texture* getColorBuffer(const std::string& _name);
	Texture* getDepthBuffer();

	//creating buffers
	bool addColorBuffer2D(const std::string& _name, GLint _glinternalformat = GL_RGBA8, GLenum _glformat = GL_RGBA, GLenum _gltype = GL_UNSIGNED_BYTE, GLboolean _multisample = false, GLint _samples = 0);
	bool setDepthBuffer2D(GLint _glinternalformat = GL_DEPTH_COMPONENT24, GLenum _glformat = GL_DEPTH_COMPONENT, GLenum _gltype = GL_FLOAT, GLboolean _multisample = false, GLint _samples = 0);

	//cube map buffers
	bool addColorBufferCB(const std::string& _name, GLint _glinternalformat = GL_RGBA8, GLenum _glformat = GL_RGBA, GLenum _gltype = GL_UNSIGNED_BYTE);
	bool setDepthBufferCB(GLint _glinternalformat = GL_DEPTH_COMPONENT24, GLenum _glformat = GL_DEPTH_COMPONENT, GLenum _gltype = GL_FLOAT);

	//optional msaa resolve stuff, in case of nullptr the current framebuffer will be blit into the default framebuffer
	bool resolve(FrameBuffer* _resolvebuffer = nullptr);

	bool complete(); //adds missing renderbuffers etc.

	//some state queries
	bool isComplete();
	bool isAllocated() { return m_isallocated; }
	bool isBound() { return m_isbound; }

private:
	//the fbo handle
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
	GLint m_vpxoff;
	GLint m_vpyoff;

	bool m_isallocated;
	bool m_isbound;
	GLenum m_glframebuffertarget;

	bool checkfbostate();
};

