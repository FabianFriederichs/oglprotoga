#include "Image2D.h"


Image2D::Image2D() :
	m_data(),
	m_level(0),
	m_glinternalformat(0),
	m_glformat(0),
	m_gltype(0),
	m_components(0),
	m_sizex(0),
	m_sizey(0)
{

}

Image2D::Image2D(const Image2D& _other) :
	m_data(_other.m_data),
	m_level(_other.m_level),
	m_glinternalformat(_other.m_glinternalformat),
	m_glformat(_other.m_glformat),
	m_gltype(_other.m_gltype),
	m_components(_other.m_components),
	m_sizex(_other.m_sizex),
	m_sizey(_other.m_sizey)
{

}

Image2D::Image2D(const GLint _glinternalformat, const GLenum _glformat, const GLenum _gltype, const DWORD _components, const GLsizei _width, const GLsizei _height, const GLuint _level, const std::vector<unsigned char>& _data) :
	m_data(_data),
	m_level(_level),
	m_glinternalformat(_glinternalformat),
	m_glformat(_glformat),
	m_gltype(_gltype),
	m_components(_components),
	m_sizex(_width),
	m_sizey(_height)
{

}


Image2D::~Image2D()
{
	m_data.clear();
}
