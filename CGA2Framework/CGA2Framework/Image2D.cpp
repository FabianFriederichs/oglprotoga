#include "Image2D.h"


Image2D::Image2D() :
	m_data(),
	m_level(0),
	m_format(0),
	m_components(0),
	m_sizex(0),
	m_sizey(0)
{

}

Image2D::Image2D(const Image2D& _other) :
	m_data(_other.m_data),
	m_level(_other.m_level),
	m_format(_other.m_format),
	m_components(_other.m_components),
	m_sizex(_other.m_sizex),
	m_sizey(_other.m_sizey),
	m_texformat(_other.m_texformat)
{

}

Image2D::Image2D(const DWORD _format, const DWORD _components, const GLsizei _width, const GLsizei _height, const GLuint _level, const std::vector<unsigned char>& _data) :
	m_data(_data),
	m_level(_level),
	m_format(_format),
	m_components(_components),
	m_sizex(_width),
	m_sizey(_height)
{

}

Image2D::Image2D(const DWORD _format, const TEXFORMAT _texformat, const DWORD _components, const GLsizei _width, const GLsizei _height, const GLuint _level, const std::vector<unsigned char>& _data) :
m_data(_data),
m_level(_level),
m_format(_format),
m_components(_components),
m_sizex(_width),
m_sizey(_height),
m_texformat(_texformat)
{

}


Image2D::~Image2D()
{
	m_data.clear();
}
