#pragma once
#include "headers.h"
#include "Texture.h"
#include "Image2D.h"
class Texture2D :
	public Texture
{
public:
	Texture2D();
	Texture2D(const Texture2D& _other) = delete;
	Texture2D(const std::string& _name);
	Texture2D(const GLint _width, const GLint _height, const std::string& _name);
	~Texture2D();

	//_empty = true: data comes not from file, the texture was a rendertarget or will be a rendertarget; _empty = false: the texture was read from file and the contained data should be buffered
	bool buffer(bool _empty);	//created gl texture can be accessed via getGLTexture()
	bool unbuffer();
	bool bind();
	bool bindToTextureUnit(const GLuint _unit);
	bool unbind();

	//getters / setters
	GLint getWidth() { return m_width; }
	GLint getHeight() { return m_height; }
	void setWidth(const GLint _width) { m_width = _width; }
	void setHeight(const GLint _height) { m_height = _height; }


	GLint getGLInternalFormat() const { return m_glinternalformat; }
	GLenum getGLFormat() const { return m_glformat; }
	GLenum getGLType() const { return m_gltype; }
	GLint getMipMapCount() const { return m_data.size(); }

	GLenum getWrapModeS() const { return m_wrapmodes; }
	GLenum getWrapModeT() const { return m_wrapmodet; }
	GLenum getMinFilter() const { return m_minfilter; }
	GLenum getMagFilter() const { return m_magfilter; }

	bool isMultiSampled() { return m_multisample; }
	GLint getMultiSampleCount() { return m_samples; }

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

	void setMultisampling(bool _multisample, GLint _samples)
	{
		if (!m_isbound && !m_isbuffered)
		{
			m_multisample = _multisample;
			m_samples = _samples;
		}
	}

	void addMipMap(const Image2D& _image);


protected:
	std::vector<Image2D> m_data; // Texture2D has 1 face and a arbitrary number of mipmaps
	//format stuff
	GLint m_width;	
	GLint m_height;
	GLint m_glinternalformat;
	GLenum m_glformat;
	GLenum m_gltype;
	bool m_multisample;
	GLint m_samples;

	//texture binding options
	GLenum m_wrapmodes;
	GLenum m_wrapmodet;
	GLenum m_minfilter;
	GLenum m_magfilter;
};

