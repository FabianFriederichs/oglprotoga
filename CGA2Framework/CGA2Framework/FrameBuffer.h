#pragma once
#include "headers.h"
#include "Texture.h"
#include "Texture2D.h"
#include "TextureCB.h"
#include "glerror.h"

typedef enum {
	FREAD,
	FWRITE,
	FREADWRITE
} FBO_BINDINGMODE;

enum class FBTYPE {
	FBT_2D,
	FBT_2D_MULTISAMPLE,
	FBT_CUBEMAP
};

class FrameBuffer
{
private:
	//this class is necessary to hold the attachment index
	class Attachment
	{
	public:
		Attachment()
		{
		}

		Attachment(const GLint _aid, Texture* _tex) :
			aid(_aid),
			tex(tex),
			renderbufferhandle(0)
		{
		}

		Attachment(const GLint _aid, GLint _rbuf) :
			aid(_aid),
			tex(nullptr),
			renderbufferhandle(_rbuf)
		{
		}

		~Attachment()
		{
			//delete texture?
		}
		GLint aid;
		Texture* tex;
		GLuint renderbufferhandle;
	};
public:
	FrameBuffer(const FBTYPE _type, const GLint _samples = 0);
	FrameBuffer(const GLint _vpxoff, const GLint _vpyoff, const GLint _vpwidth, const GLint _vpheight, const FBTYPE _type, const GLint _samples = 0);
	~FrameBuffer();

	bool allocate();
	bool destroy();
	bool bind(const FBO_BINDINGMODE _bindingmode);
	bool unbind();
	bool updateGLViewport();
	bool updateGLViewport(const GLint _vpxoff, const GLint _vpyoff, const GLint _vpwidth, const GLint _vpheight);
	
	//blit operations
	//blit one fbo to another //if width or height is negative, the viewport dimensions are used
	static bool blit(FrameBuffer* _source,
		FrameBuffer* _target,
		const GLint _xoff,
		const GLint _yoff,
		const GLint _width,
		const GLint _height,
		bool colorbit,
		GLint _colorbufferindex,
		bool _depthbit,
		bool _stencilbit);

	//blit the default framebuffer to a fbo
	static bool blitdefault(bool _frontback,	//true: frontbuffer, flase: backbuffer
		FrameBuffer* _target,
		const GLint _xoff,
		const GLint _yoff,
		const GLint _width,
		const GLint _height,
		bool _depthbit,
		bool _stencilbit);

	//getters / setters
	GLuint getGLFBO() { return m_fbo; }
	GLint getViewportWidth() { return m_vpwidth; }
	GLint getViewportHeight() { return m_vpheight; }

	//access the textures
	Texture* getColorBufferTex(const std::string& _name);
	Texture* getDepthBufferTex();

	GLint getColorRenderBuffer(const std::string& _name);
	GLint getDepthRenderBuffer();

	//creating buffers
	bool addColorBufferTex(const std::string& _name, GLint _glinternalformat = GL_RGBA8, GLenum _glformat = GL_RGBA, GLenum _gltype = GL_UNSIGNED_BYTE);
	bool setDepthBufferTex(GLint _glinternalformat = GL_DEPTH_COMPONENT24, GLenum _glformat = GL_DEPTH_COMPONENT, GLenum _gltype = GL_FLOAT);

	//creating buffers
	//bool addColorRenderBuffer(const std::string& _name, GLint _glinternalformat = GL_RGBA8, GLenum _glformat = GL_RGBA, GLenum _gltype = GL_UNSIGNED_BYTE);
	//bool setDepthRenderBuffer(GLint _glinternalformat = GL_DEPTH_COMPONENT24, GLenum _glformat = GL_DEPTH_COMPONENT, GLenum _gltype = GL_FLOAT);

	//optional msaa resolve stuff, in case of nullptr the current framebuffer will be blit into the default framebuffer
	bool resolve(FrameBuffer* _resolvebuffer = nullptr);

	bool complete(GLenum _depthinternalformat = GL_DEPTH24_STENCIL8, GLenum _colorinternalformat = GL_RGBA8); //adds missing renderbuffers etc. and attaches the added images

	//some state queries
	bool isComplete() { return checkfbostate(); }
	bool isAllocated() { return m_isallocated; }
	bool isBound() { return m_isbound; }

private:
	//the fbo handle
	GLuint m_fbo;
	//when not using a buffer for a texture, keep handles to the renderbuffers
	//std::unordered_map<std::string, Attachment> m_colorrenderbuffers;
	//Attachment m_depthrenderbuffer;	
	//the actual textures
	std::unordered_map<std::string, Attachment> m_colorbuffers;
	Attachment m_depthbuffer;

	//viewport
	GLint m_vpwidth;
	GLint m_vpheight;
	GLint m_vpxoff;
	GLint m_vpyoff;

	bool m_isallocated;
	bool m_isbound;
	GLenum m_glframebuffertarget;

	//framebuffer purpose
	FBTYPE m_type;
	GLint m_samples;

	bool checkfbostate();

	
};

