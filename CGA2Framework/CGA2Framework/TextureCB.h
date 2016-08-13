#pragma once
#include "Texture.h"
#include "Image2D.h"
#include "glerror.h"
class TextureCB :
	public Texture
{
public:
	//typedefs
	typedef enum {
		CBFACE_POSX,
		CBFACE_NEGX,
		CBFACE_POSY,
		CBFACE_NEGY,
		CBFACE_POSZ,
		CBFACE_NEGZ
	} CBFACE;

	TextureCB();
	TextureCB(const std::string& _name);
	TextureCB(const GLint _width, const GLint _height, const std::string& _name);
	~TextureCB();

	//_empty = true: data comes not from file, the texture was a rendertarget or will be a rendertarget; _empty = false: the texture was read from file and the contained data should be buffered
	bool buffer(bool _empty) { return false; }	//created gl texture can be accessed via getGLTexture()
	bool unbuffer() { return false; }
	bool bind() { return false; }
	bool bindToTextureUnit(const GLuint _unit) { return false; }
	bool unbind() { return false; }

	GLint getGLInternalFormat() const { return m_glinternalformat; }
	GLenum getGLFormat() const { return m_glformat; }
	GLenum getGLType() const { return m_gltype; }
	GLint getMipMapCount(GLint _faceindex) { return 1; }

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

	void addMipMap(CBFACE _face, const Image2D& _image) {  }


protected:
	std::vector<Image2D> m_datapx;	//Positive x mipmaps
	std::vector<Image2D> m_datanx;	//negative x mipmaps
	std::vector<Image2D> m_datapy;	//positive y mipmaps
	std::vector<Image2D> m_datany;	//negative y mipmaps
	std::vector<Image2D> m_datapz;	//positive z mipmaps
	std::vector<Image2D> m_datanz;	//negative z mipmaps
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

