#include "TextureCB.h"

TextureCB::TextureCB() :
Texture(TEX_CUBEMAP),
m_datapx(),
m_datanx(),
m_datapy(),
m_datany(),
m_datapz(),
m_datanz(),
m_width(0),
m_height(0),
m_glinternalformat(0),
m_glformat(0),
m_gltype(0),
m_wrapmodes(GL_REPEAT),
m_wrapmodet(GL_REPEAT),
m_minfilter(GL_NEAREST),
m_magfilter(GL_NEAREST)
{

}

TextureCB::TextureCB(const std::string& _name) :
Texture(_name, TEX_CUBEMAP),
m_datapx(),
m_datanx(),
m_datapy(),
m_datany(),
m_datapz(),
m_datanz(),
m_width(0),
m_height(0),
m_glinternalformat(0),
m_glformat(0),
m_gltype(0),
m_wrapmodes(GL_REPEAT),
m_wrapmodet(GL_REPEAT),
m_minfilter(GL_NEAREST),
m_magfilter(GL_NEAREST)
{
}

TextureCB::TextureCB(const GLint _width, const std::string& _name) :
Texture(_name, TEX_CUBEMAP),
m_datapx(),
m_datanx(),
m_datapy(),
m_datany(),
m_datapz(),
m_datanz(),
m_width(_width),
m_height(_width),
m_glinternalformat(0),
m_glformat(0),
m_gltype(0),
m_wrapmodes(GL_REPEAT),
m_wrapmodet(GL_REPEAT),
m_minfilter(GL_NEAREST),
m_magfilter(GL_NEAREST)
{

}


TextureCB::~TextureCB()
{
	if (isBound())
	{
		unbind();
	}
	if (isBuffered())
	{
		unbuffer();
	}
	m_datapx.clear();
	m_datanx.clear();
	m_datapy.clear();
	m_datany.clear();
	m_datapz.clear();
	m_datanz.clear();
}

void TextureCB::addMipMap(CBFACE _face, const Image2D& _image)
{
	switch (_face)
	{
	case CBFACE_POSX:
		m_datapx.push_back(_image);
		break;
	case CBFACE_NEGX:
		m_datanx.push_back(_image);
		break;
	case CBFACE_POSY:
		m_datapy.push_back(_image);
		break;
	case CBFACE_NEGY:
		m_datany.push_back(_image);
		break;
	case CBFACE_POSZ:
		m_datapz.push_back(_image);
		break;
	case CBFACE_NEGZ:
		m_datanz.push_back(_image);
		break;
	}
}

bool TextureCB::buffer(bool _empty)
{
	if (m_texture == 0)
	{
		if (!_empty && m_datapx.size() > 0 && m_datanx.size() > 0 && m_datapy.size() > 0 && m_datany.size() > 0 && m_datapz.size() > 0 && m_datanz.size() > 0)	//at least one mipmap level //no multisampling supported here
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
				glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrapmodes);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrapmodet);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrapmoder);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_minfilter);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_magfilter);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
				
				GLint maxmipmaplevel = 10000;
				maxmipmaplevel = min((GLint)m_datapx.size() - 1, maxmipmaplevel);
				maxmipmaplevel = min((GLint)m_datanx.size() - 1, maxmipmaplevel);
				maxmipmaplevel = min((GLint)m_datapy.size() - 1, maxmipmaplevel);
				maxmipmaplevel = min((GLint)m_datany.size() - 1, maxmipmaplevel);
				maxmipmaplevel = min((GLint)m_datapz.size() - 1, maxmipmaplevel);
				maxmipmaplevel = min((GLint)m_datanz.size() - 1, maxmipmaplevel);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, maxmipmaplevel);
				if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }


				//posx face
				for (size_t i = 0; i < m_datapx.size(); i++)
				{

					GLint format = m_datapx[i].getGLInternalFormat();

					if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) //dxt compressed rgba
					{
						glCompressedTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_X,
							m_datapx[i].getLevel(),
							m_datapx[i].getGLInternalFormat(),
							m_datapx[i].getSizeX(),
							m_datapx[i].getSizeY(),
							0, m_datapx[0].getData().size(),
							m_datapx[0].getData().data()
							);
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
						m_isbuffered = true;
						//return true;
					}
					else if (format == GL_RGBA8)	//uncompressed rgba8
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
							m_datapx[i].getLevel(),
							GL_RGBA8,
							m_datapx[i].getSizeX(),
							m_datapx[i].getSizeY(),
							0,
							m_datapx[i].getGLFormat(),
							m_datapx[i].getGLType(),
							m_datapx[i].getData().data());
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
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

				//negx face
				for (size_t i = 0; i < m_datanx.size(); i++)
				{

					GLint format = m_datanx[i].getGLInternalFormat();

					if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) //dxt compressed rgba
					{
						glCompressedTexImage2D(
							GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
							m_datanx[i].getLevel(),
							m_datanx[i].getGLInternalFormat(),
							m_datanx[i].getSizeX(),
							m_datanx[i].getSizeY(),
							0, m_datanx[0].getData().size(),
							m_datanx[0].getData().data()
							);
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
						m_isbuffered = true;
						//return true;
					}
					else if (format == GL_RGBA8)	//uncompressed rgba8
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
							m_datanx[i].getLevel(),
							GL_RGBA8,
							m_datanx[i].getSizeX(),
							m_datanx[i].getSizeY(),
							0,
							m_datanx[i].getGLFormat(),
							m_datanx[i].getGLType(),
							m_datanx[i].getData().data());
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
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

				//posy face
				for (size_t i = 0; i < m_datapy.size(); i++)
				{

					GLint format = m_datapy[i].getGLInternalFormat();

					if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) //dxt compressed rgba
					{
						glCompressedTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
							m_datapy[i].getLevel(),
							m_datapy[i].getGLInternalFormat(),
							m_datapy[i].getSizeX(),
							m_datapy[i].getSizeY(),
							0, m_datapy[0].getData().size(),
							m_datapy[0].getData().data()
							);
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
						m_isbuffered = true;
						//return true;
					}
					else if (format == GL_RGBA8)	//uncompressed rgba8
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
							m_datapy[i].getLevel(),
							GL_RGBA8,
							m_datapy[i].getSizeX(),
							m_datapy[i].getSizeY(),
							0,
							m_datapy[i].getGLFormat(),
							m_datapy[i].getGLType(),
							m_datapy[i].getData().data());
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
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

				//negy face
				for (size_t i = 0; i < m_datany.size(); i++)
				{

					GLint format = m_datany[i].getGLInternalFormat();

					if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) //dxt compressed rgba
					{
						glCompressedTexImage2D(
							GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
							m_datany[i].getLevel(),
							m_datany[i].getGLInternalFormat(),
							m_datany[i].getSizeX(),
							m_datany[i].getSizeY(),
							0, m_datany[0].getData().size(),
							m_datany[0].getData().data()
							);
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
						m_isbuffered = true;
						//return true;
					}
					else if (format == GL_RGBA8)	//uncompressed rgba8
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
							m_datany[i].getLevel(),
							GL_RGBA8,
							m_datany[i].getSizeX(),
							m_datany[i].getSizeY(),
							0,
							m_datany[i].getGLFormat(),
							m_datany[i].getGLType(),
							m_datany[i].getData().data());
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
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

				//posz face
				for (size_t i = 0; i < m_datapz.size(); i++)
				{

					GLint format = m_datapz[i].getGLInternalFormat();

					if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) //dxt compressed rgba
					{
						glCompressedTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
							m_datapz[i].getLevel(),
							m_datapz[i].getGLInternalFormat(),
							m_datapz[i].getSizeX(),
							m_datapz[i].getSizeY(),
							0, m_datapz[0].getData().size(),
							m_datapz[0].getData().data()
							);
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
						m_isbuffered = true;
						//return true;
					}
					else if (format == GL_RGBA8)	//uncompressed rgba8
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
							m_datapz[i].getLevel(),
							GL_RGBA8,
							m_datapz[i].getSizeX(),
							m_datapz[i].getSizeY(),
							0,
							m_datapz[i].getGLFormat(),
							m_datapz[i].getGLType(),
							m_datapz[i].getData().data());
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
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

				//negz face
				for (size_t i = 0; i < m_datanz.size(); i++)
				{

					GLint format = m_datanz[i].getGLInternalFormat();

					if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) //dxt compressed rgba
					{
						glCompressedTexImage2D(
							GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
							m_datanz[i].getLevel(),
							m_datanz[i].getGLInternalFormat(),
							m_datanz[i].getSizeX(),
							m_datanz[i].getSizeY(),
							0, m_datanz[0].getData().size(),
							m_datanz[0].getData().data()
							);
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
						m_isbuffered = true;
						//return true;
					}
					else if (format == GL_RGBA8)	//uncompressed rgba8
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
							m_datanz[i].getLevel(),
							GL_RGBA8,
							m_datanz[i].getSizeX(),
							m_datanz[i].getSizeY(),
							0,
							m_datanz[i].getGLFormat(),
							m_datanz[i].getGLType(),
							m_datanz[i].getData().data());
						if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
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
		else if (_empty)	//this is an empty texture -> allocate opengl memory
		{			
			glGenTextures(1, &m_texture);
			if (m_texture == 0 || checkglerror())
			{
				std::cerr << "TEXTURE BUFFERING ERROR: gl texture object creation failed." << std::endl;
				m_isbuffered = false;
				m_texture = 0;
				return false;
			}

			glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrapmodes);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrapmodet);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrapmoder);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_minfilter);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_magfilter);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }

			if (m_glinternalformat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || m_glinternalformat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || m_glinternalformat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
			{
				std::cout << "TEXTURE BUFFERING ERROR: Compressed texture formats are not supported for empty/rendertextures." << std::endl;
				m_isbuffered = false;
				m_texture = 0;
				return false;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				0,
				m_glinternalformat,
				m_width,
				m_height,
				0,
				m_glformat,
				m_gltype,
				NULL);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				0,
				m_glinternalformat,
				m_width,
				m_height,
				0,
				m_glformat,
				m_gltype,
				NULL);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				0,
				m_glinternalformat,
				m_width,
				m_height,
				0,
				m_glformat,
				m_gltype,
				NULL);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				0,
				m_glinternalformat,
				m_width,
				m_height,
				0,
				m_glformat,
				m_gltype,
				NULL);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				0,
				m_glinternalformat,
				m_width,
				m_height,
				0,
				m_glformat,
				m_gltype,
				NULL);
			if (checkglerror()) { m_isbuffered = false;  glDeleteTextures(1, &m_texture); m_texture = 0; GLERR return false; }
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				0,
				m_glinternalformat,
				m_width,
				m_height,
				0,
				m_glformat,
				m_gltype,
				NULL);

			if (checkglerror())
			{
				std::cout << "TEXTURE BUFFERING ERROR: An error occured while buffering the texture." << std::endl;
				m_isbuffered = false;
				glDeleteTextures(1, &m_texture);
				m_texture = 0;
				return false;
			}

			unbind();

			m_isbuffered = true;
			return true;			
		}
		else
		{
			std::cout << "TEXTURE BUFFERING ERROR: No texture data. Non-empty textures must have at least one mipmap." << std::endl;
			m_isbuffered = false;
			m_texture = 0;
			return false;
		}
	}
	else
	{
		std::cout << "TEXTURE BUFFERING WARNING: The texture is already buffered." << std::endl;
		return false;
	}
}

bool TextureCB::unbuffer()
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
	else
	{
		return false;
	}
}

bool TextureCB::bind()
{
	if (m_texture != 0 && isBuffered())
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		
		if (checkglerror())
		{
			std::cout << "Error binding texture.\n";
			m_isbound = false;
			return false;
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrapmodes);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrapmodet);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrapmoder);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_minfilter);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_magfilter);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			m_isbound = true;
			return true;
		}		
	}
	else
	{
		std::cout << "Error binding texture. OpenGL texture is 0.\n";
		return false;
	}
}

bool TextureCB::bindToTextureUnit(const GLuint _unit)
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
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		if (checkglerror())
		{
			std::cout << "Error binding texture.\n";
			m_isbound = false;
			return false;
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrapmodes);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrapmodet);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrapmoder);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_minfilter);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_magfilter);
			if (checkglerror()) { m_isbound = false; GLERR return false; }
			m_isbound = true;
			return true;
		}		
	}
	else
	{
		std::cout << "Error binding texture. OpenGL texture is 0.\n";
		return false;
	}
}

bool TextureCB::unbind()
{	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	if (checkglerror())
	{
		return false;
	}
	else
	{
		return true;
	}	
}
