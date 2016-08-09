#include "Texture.h"

Texture::Texture() :
	m_filepath(),
	m_texture(0),
	m_name(""),
	m_sizex(0),
	m_sizey(0),
	m_id(IDProvider::createID()),
	m_isloaded(false),
	m_isbound(false),
	m_isbuffered(false),
	m_isrendertarget(false)
{
	
}

Texture::Texture(const Texture& _other) :
	m_filepath(_other.m_filepath),
	m_texture(0),				//a copy of a Texture object should create a seperate gl texture!
	m_name(_other.m_name),
	m_sizex(_other.m_sizex),
	m_sizey(_other.m_sizey),
	m_id(IDProvider::createID()),
	m_isloaded(_other.m_isloaded),
	m_isbound(_other.m_isbound),
	m_isbuffered(_other.m_isbuffered),
	m_isrendertarget(_other.m_isrendertarget)
{

}

Texture::Texture(const std::string& _filepath) :
	m_filepath(_filepath),
	m_texture(0),
	m_name(""),
	m_sizex(0),
	m_sizey(0),
	m_id(IDProvider::createID()),
	m_isloaded(false),
	m_isbound(false),
	m_isbuffered(false),
	m_isrendertarget(false)
{

}

Texture::Texture(const std::string& _filepath, const std::string& _name) :
	m_filepath(_filepath),
	m_texture(0),
	m_name(_name),
	m_sizex(0),
	m_sizey(0),
	m_id(IDProvider::createID()),
	m_isloaded(false),
	m_isbound(false),
	m_isbuffered(false),
	m_isrendertarget(false)
{

}

//Texture::Texture(const bool isRenderTarget, GLsizei _width, GLsizei _height) :
//	m_filepath(""),
//	m_texture(0),
//	m_name(""),
//	m_sizex(_width),
//	m_sizey(_height),
//	m_id(IDProvider::createID()),
//	m_isloaded(true),
//	m_isbound(false),
//	m_isbuffered(false),
//	m_isrendertarget(true)
//{
//
//}

//Texture::Texture(const bool isRenderTarget, GLsizei _width, GLsizei _height, const std::string& _name) :
//m_filepath(""),
//m_texture(0),
//m_name(_name),
//m_sizex(_width),
//m_sizey(_height),
//m_id(IDProvider::createID()),
//m_isloaded(true),
//m_isbound(false),
//m_isbuffered(false),
//m_isrendertarget(true)
//{
//
//}

Texture::~Texture()
{
	m_data.clear();
	freeGLTexture();
}

bool Texture::loadData()
{
	m_data.clear();
	if (DDSLoader::loadDDSTex(m_filepath, *this))
	{
		std::cerr << "Texture loading was successful." << std::endl;
		m_isloaded = true;
		return true;
	}
	else
	{
		std::cerr << "Texture could not be loaded." << std::endl;
		m_isloaded = false;
		return false;
	}
	m_isloaded = false;
	return false;
}

bool Texture::unloadData()
{
	m_data.clear();
	m_isloaded = false;
	return true;
}

bool Texture::prepareAsRenderTarget(GLsizei _width, GLsizei _height, GLenum _internalformat, GLenum _format)
{
	m_data.clear();
	m_isloaded = false;
	m_isbound = false;
	m_isbuffered = false;
	m_texture = 0;

	m_rtinternalformat = _internalformat;
	m_rtformat = _format;

	m_isrendertarget = true;
	m_type = TEXTYPE::TEX_2D;
	return true;
}

bool Texture::loadGLTexture(GLenum _wrapmodes, GLenum _wrapmodet, GLenum _minfilter, GLenum _magfilter)
{
	switch (m_type)
	{
		case TEX_2D:
			if (m_texture == 0)
			{
				if (!m_isrendertarget)
				{
					if (m_data.size() == 1)	//1 face for 2D texture
					{
						if (m_data[0].size() > 0)	//at least one mipmap level
						{
							glGenTextures(1, &m_texture);
							if (m_texture == 0)
							{
								std::cerr << "gl texture object creation failed." << std::endl;
								m_isbuffered = false;
								return false;
							}
							else
							{
								glBindTexture(GL_TEXTURE_2D, m_texture); GLERR
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapmodes); GLERR
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapmodet); GLERR
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minfilter); GLERR
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magfilter); GLERR
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_data[0].size() - 1);

								for (size_t i = 0; i < m_data[0].size(); i++)
								{
									TEXFORMAT tf = m_data[0][i].getTexFormat();
									switch (tf)
									{ 
										case TF_DXT:
											glCompressedTexImage2D(
												GL_TEXTURE_2D,
												m_data[0][i].getLevel(),
												m_data[0][i].getFormat(),
												m_data[0][i].getSizeX(),
												m_data[0][i].getSizeY(),
												0, m_data[0][i].getData().size(),
												m_data[0][i].getData().data()
												); GLERR
												break;
										case TF_ARGB8888:
											glTexImage2D(GL_TEXTURE_2D,
												m_data[0][i].getLevel(),
												GL_RGBA8,
												m_data[0][i].getSizeX(),
												m_data[0][i].getSizeY(),
												0,
												GL_RGBA,
												GL_UNSIGNED_BYTE,
												m_data[0][i].getData().data()); GLERR
											break;
										case TF_RGBA32F:
											std::cout << "RGBA32F format is not supported yet.\n";
											m_isbuffered = false;
											break;
										default:
											std::cout << "Image2D must have TF_DXT, TF_ARGB8888 or TF_RGBA32F format to be bound.\n";
											m_isbuffered = false;
									}
								}

								//glBindTexture(GL_TEXTURE_2D, 0);

								
								if (glGetError() == GL_NO_ERROR)
								{
									m_isbuffered = true;
									return true;
								}
								else
								{
									
									m_isbuffered = false;
									return false;
								}
							}
						}
						else
						{
							std::cerr << "No texture data available." << std::endl;
							m_isbuffered = false;
							return false;
						}
					}
					else
					{
						std::cerr << "Wrong face count for 2D texture." << std::endl;
						m_isbuffered = false;
						return false;
					}
				}
				else
				{

					glGenTextures(1, &m_texture);
					if (m_texture == 0)
					{
						std::cerr << "gl texture object creation failed." << std::endl;
						m_isbuffered = false;
						return false;
					}
					else
					{
						glBindTexture(GL_TEXTURE_2D, m_texture); GLERR
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapmodes); GLERR
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapmodet); GLERR
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minfilter); GLERR
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magfilter); GLERR
							//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);

							glTexImage2D(GL_TEXTURE_2D, 0, m_rtinternalformat, m_sizex, m_sizey, 0, m_rtformat, GL_FLOAT, NULL); GLERR

							//glBindTexture(GL_TEXTURE_2D, 0);

						if (glGetError() == GL_NO_ERROR)
						{
							m_isbuffered = true;
							return true;
						}
						else
						{
							m_isbuffered = false;
							return false;
						}
					}
				}
					
			}
			else
			{
				std::cerr << "The texture is already bound to opengl. Call unloadGLTexture to unbind it first!" << std::endl;
				m_isbuffered = false;
				return false;
			}
			break;
		case TEX_3D:
			std::cerr << "Error. 3D textures are not supported yet." << std::endl;
			m_isbuffered = false;
			break;
		case TEX_CUBEMAP:
			std::cerr << "Error. Cubemap textures are not supported yet." << std::endl;
			m_isbuffered = false;
			return false;
			break;
		case TEX_ARRAY:
			std::cerr << "Error. Array textures are not supported yet." << std::endl;
			m_isbuffered = false;
			return false;
			break;
		case TEX_DX10_CUBEMAP:
			std::cerr << "Error. DX10 cubemap textures are not supported yet." << std::endl;
			m_isbuffered = false;
			return false;
			break;
		default:
			std::cerr << "Error. Invalid texture type." << std::endl;
			m_isbuffered = false;
			return false;
			break;
	}
	m_isbuffered = false;
	return false;
}

bool Texture::freeGLTexture()
{
	glDeleteTextures(1, &m_texture); GLERR
	m_isbuffered = false;
	return true;
}

bool Texture::bindToTextureUnit(GLuint _unit)
{
	if (m_texture != 0)
	{
		switch (m_type)
		{
		case TEX_2D:
			glActiveTexture(GL_TEXTURE0 + _unit); GLERR
			glBindTexture(GL_TEXTURE_2D, m_texture); GLERR
			if (glGetError() == GL_NO_ERROR)
			{
				m_isbound = true;
				return true;
			}
			else
			{
				m_isbound = false;
				return false;
			}
			break;
		case TEX_3D:
			std::cerr << "Error. 3D textures are not supported yet." << std::endl;
			m_isbound = false;
			return false;
			break;
		case TEX_ARRAY:
			std::cerr << "Error. Array textures are not supported yet." << std::endl;
			m_isbound = false;
			return false;
			break;
		case TEX_CUBEMAP:
			std::cerr << "Error. Cubemap textures are not supported yet." << std::endl;
			m_isbound = false;
			return false;
			break;
		case TEX_DX10_CUBEMAP:
			std::cerr << "Error. DX10 cubemap textures are not supported yet." << std::endl;
			m_isbound = false;
			return false;
			break;
		default:
			std::cerr << "Error. Invalid texture type." << std::endl;
			m_isbound = false;
			return false;
			break;
		}
	}
	m_isbound = false;
	return false;
}




