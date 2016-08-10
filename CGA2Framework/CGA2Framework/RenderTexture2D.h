#pragma once
#include "Texture.h"
class RenderTexture2D :
	public Texture
{
public:
	RenderTexture2D();
	RenderTexture2D(const std::string& _name);
	~RenderTexture2D();

	//buffering and binding
	virtual bool buffer() = 0;	//created gl texture can be accessed via getGLTexture()
	virtual bool unbuffer() = 0;
	virtual bool bind() = 0;
	virtual bool bindToTextureUnit(const GLuint _unit) = 0;
	virtual bool unbind() = 0;

	//getters / setters
	GLint getWidth() { return m_width; }
	GLint getHeight() { return m_height; }
	void setWidth(const GLint _width) { m_width = _width; }
	void setHeight(const GLint _height) { m_height = _height; }


	GLint getGLInternalFormat() const { return m_glinternalformat; }
	GLenum getGLFormat() const { return m_glformat; }
	GLenum getGLType() const { return m_gltype; }

	GLenum getWrapModeS() const { return m_wrapmodes; }
	GLenum getWrapModeT() const { return m_wrapmodet; }
	GLenum getMinFilter() const { return m_minfilter; }
	GLenum getMagFilter() const { return m_magfilter; }

	void setGLInternalFormat(const GLint _format) { m_glinternalformat = _format; }
	void setGLFormat(const GLenum _format) { m_glformat = _format; }
	void setGLType(const GLenum _type) { m_gltype = _type; }

	void setWrapModeS(const GLenum _wraptmodes) { m_wrapmodes = _wraptmodes; }
	void setWrapModeT(const GLenum _wraptmodet) { m_wrapmodet = _wraptmodet; }
	void setMinFilter(const GLenum _minfilter) { m_minfilter = _minfilter; }
	void setMagFilter(const GLenum _maxfilter) { m_magfilter = _maxfilter; }

	void setBindingOptions(const GLenum _wraptmodes, const GLenum _wraptmodet, const GLenum _minfilter, const GLenum _maxfilter)
	{
		m_wrapmodes = _wraptmodes;
		m_wrapmodet = _wraptmodet;
		m_minfilter = _minfilter;
		m_magfilter = _maxfilter;
	}


protected:
	GLint m_width;
	GLint m_height;

	//format
	GLint m_glinternalformat;
	GLenum m_glformat;
	GLenum m_gltype;

	//texture binding options
	GLenum m_wrapmodes;
	GLenum m_wrapmodet;
	GLenum m_minfilter;
	GLenum m_magfilter;
};

