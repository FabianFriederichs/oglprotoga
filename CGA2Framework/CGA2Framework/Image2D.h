#pragma once
#include "headers.h"
#include "dds.h"
#include "glerror.h"

typedef enum {
	TF_DXT = 0,
	TF_ARGB8888 = 1,
	TF_RGBA32F = 2
} TEXFORMAT;


class Image2D
{
public:
	Image2D();
	Image2D(const Image2D& _other);
	Image2D(const GLint _glinternalformat, const GLenum _glformat, const GLenum _gltype, const DWORD _components, const GLsizei _width, const GLsizei _height, const GLuint _level, const std::vector<unsigned char>& _data);
	~Image2D();

	GLint getGLInternalFormat() const { return m_glinternalformat; }
	GLenum getGLFormat() const { return m_glformat; }
	GLenum getGLType() const { return m_gltype; }
	const DWORD getComponents() const { return m_components; }
	const GLint getLevel() const { return m_level; }
	const GLsizei getSizeX() const { return m_sizex; }
	const GLsizei getSizeY() const { return m_sizey; }
	const std::vector<unsigned char>& getData() { return m_data; }

	void setGLInternalFormat(const GLint _format) { m_glinternalformat = _format; }
	void setGLFormat(const GLenum _format) { m_glformat = _format; }
	void setGLType(const GLenum _type) { m_gltype = _type; }
	void setComponents(const DWORD _components) { m_components = _components; }
	void setSize(const GLsizei _width, const GLsizei _height) { m_sizex = _width; m_sizey = _height; }
	void setLevel(const GLuint _level) { m_level = _level; }
	void setData(const std::vector<unsigned char>& _data) { m_data = _data; }

private:	
	std::vector<unsigned char> m_data;
	GLuint m_level;
	GLint m_glinternalformat;
	GLenum m_glformat;
	GLenum m_gltype;
	DWORD m_components;
	GLsizei m_sizex;
	GLsizei m_sizey;

};

