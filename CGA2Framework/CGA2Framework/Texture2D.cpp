#include "Texture2D.h"


Texture2D::Texture2D() :
	Texture(TEX_2D),
	m_data(),
	m_width(0),
	m_height(0),
	m_glinternalformat(0),
	m_glformat(0),
	m_gltype(0)
{

}

Texture2D::Texture2D(const std::string& _name) :
	Texture(_name, TEX_2D),
	m_data(),
	m_width(0),
	m_height(0),
	m_glinternalformat(0),
	m_glformat(0),
	m_gltype(0)
{

}

Texture2D::Texture2D(const GLint _width, const GLint _height, const std::string& _name) :
Texture(_name, TEX_2D),
m_data(),
m_width(_width),
m_height(_height),
m_glinternalformat(0),
m_glformat(0),
m_gltype(0)
{

}


Texture2D::~Texture2D()
{
	if (isBound())
	{
		unbind();
	}
	if (isBuffered())
	{
		unbuffer();
	}	
	m_data.clear();
}

void Texture2D::addMipMap(const Image2D& _image)
{
	m_data.push_back(_image);
}

bool Texture2D::buffer()
{
	if (m_texture == 0)
	{
		if (m_data.size() > 0)	//at least one mipmap level
		{
			glGenTextures(1, &m_texture);
			if (m_texture == 0 || checkglerror())
			{
				std::cerr << "gl texture object creation failed." << std::endl;
				m_isbuffered = false;
				m_texture = 0;
				return false;
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, m_texture);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR; return false; }
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapmodes);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR; return false; }
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapmodet);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR; return false; }
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minfilter);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR; return false; }
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magfilter);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR; return false; }
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_data.size() - 1);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR; return false; }

				for (size_t i = 0; i < m_data.size(); i++)
				{

					GLint format = m_data[i].getGLInternalFormat();
					
					if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) //dxt compressed rgba
					{
						glCompressedTexImage2D(
							GL_TEXTURE_2D,
							m_data[i].getLevel(),
							m_data[i].getGLInternalFormat(),
							m_data[i].getSizeX(),
							m_data[i].getSizeY(),
							0, m_data[0].getData().size(),
							m_data[0].getData().data()
							);
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR; return false; }
						m_isbuffered = true;
						//return true;
					}
					else if (format == GL_RGBA8)	//uncompressed rgba8
					{
						glTexImage2D(GL_TEXTURE_2D,
							m_data[i].getLevel(),
							GL_RGBA8,
							m_data[i].getSizeX(),
							m_data[i].getSizeY(),
							0,
							m_data[i].getGLFormat(),
							m_data[i].getGLType(),
							m_data[i].getData().data());
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR; return false; }
						m_isbuffered = true;
						
						//return true;
					}
					else
					{
						std::cout << "TEXTURE BUFFERING ERROR: The format is not supported yet." << std::endl;
						m_isbuffered = false;
						glDeleteTextures(1, &m_texture); GLERR
						m_texture = 0;
						return false;
					}
				}
				unbind();

				return true;
			}
		}
		else
		{
			std::cout << "TEXTURE BUFFERING ERROR: No texture data. The texture must have at least one mipmap." << std::endl;
			m_isbuffered = false;
			m_texture = 0;
			return false;
		}
	}
	else
	{
		std::cout << "TEXTURE BUFFERING WARNING: The texture is already buffered." << std::endl;
	}
}

bool Texture2D::unbuffer()
{
	if (isBuffered())
	{
		if (isBound())
		{
			unbind();
		}
		glDeleteTextures(1, &m_texture);
		if (checkglerror())
		{
			return false;
		}
		else
		{
			m_texture = 0;
			m_isbuffered = false;
			return true;
		}
	}
}

bool Texture2D::bind()
{
	if (m_texture != 0 && isBuffered())
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		if (checkglerror())
		{
			std::cout << "Error binding texture.\n";
			m_isbound = false;
			return false;
		}
		else
		{
			m_isbound = true;
			return true;
		}
	}
	else
	{
		std::cout << "Error binding texture. OpenGL texture is 0.\n";
	}
}

bool Texture2D::bindToTextureUnit(const GLuint _unit)
{
	if (m_texture != 0 && isBuffered())
	{
		glActiveTexture(GL_TEXTURE0 + _unit);
		if (checkglerror())
		{
			std::cout << "Texture unit couldn't be activated.\n";
			m_isbound = false;
			return false;
		}
		glBindTexture(GL_TEXTURE_2D, m_texture);
		if (checkglerror())
		{
			std::cout << "Error binding texture.\n";
			m_isbound = false;
			return false;
		}
		else
		{
			m_isbound = true;
			return true;
		}
	}
	else
	{
		std::cout << "Error binding texture. OpenGL texture is 0.\n";
	}
}

bool Texture2D::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	if (checkglerror())
	{
		return false;
	}
	else
	{
		return true;
	}
}
