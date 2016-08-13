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
	~FrameBuffer();

	bool allocate() { return true; }
	bool destroy() { return true; }
	bool bind(FBO_BINDINGMODE _bindingmode) { return true; }
	bool updateGLViewport() { return true; }
	bool blit(FrameBuffer* _target) { return true; }

	//getters / setters
	GLuint getGLFBO() { return m_fbo; }
	GLint getViewportWidth() { return m_vpwidth; }
	GLint getViewportHeight() { return m_vpheight; }

	void setViewPortWidth(const GLint _width) { m_vpwidth = _width; }
	void setViewPortHeight(const GLint _height) { m_vpheight = _height; }

	//access the textures
	Texture* getColorBuffer(const std::string& name);
	Texture* getDepthBuffer();

	//creating buffers
	bool addColorBuffer2D(const std::string& _name, GLint _glinternalformat = GL_RGBA8, GLenum _glformat = GL_RGBA, GLenum _gltype = GL_UNSIGNED_BYTE, GLboolean _multisample = false, GLint _samples = 0);
	bool setDepthBuffer2D(GLint _glinternalformat = GL_DEPTH_COMPONENT24, GLenum _glformat = GL_DEPTH_COMPONENT, GLenum _gltype = GL_FLOAT, GLboolean _multisample = false, GLint _samples = 0);

	//cube map buffers
	bool addColorBufferCB(const std::string& _name, GLint _glinternalformat = GL_RGBA8, GLenum _glformat = GL_RGBA, GLenum _gltype = GL_UNSIGNED_BYTE, GLboolean _multisample = false, GLint _samples = 0);
	bool setDepthBufferCB(GLint _glinternalformat = GL_DEPTH_COMPONENT24, GLenum _glformat = GL_DEPTH_COMPONENT, GLenum _gltype = GL_FLOAT, GLboolean _multisample = false, GLint _samples = 0);

	//optional msaa resolve stuff
	bool resolve(FrameBuffer* _resolvebuffer = nullptr);
	bool resolveToTexture(Texture* _resolvetexture);

	bool complete(); //adds missing renderbuffers etc.

	

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
};

