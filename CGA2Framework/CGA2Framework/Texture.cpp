#include "Texture.h"

Texture::Texture() :
	m_filepath(),
	m_texture(0),
	m_name(""),
	m_sizex(0.0f),
	m_sizey(0.0f),
	m_id(IDProvider::getInstance().createID())	
{
	
}

Texture::Texture(const Texture& _other) :
	m_filepath(_other.m_filepath),
	m_texture(0),				//a copy of a Texture object should create a seperate gl texture!
	m_name(_other.m_name),
	m_sizex(_other.m_sizex),
	m_sizey(_other.m_sizey),
	m_id(IDProvider::getInstance().createID())
{

}

Texture::Texture(const std::string& _filepath) :
	m_filepath(_filepath),
	m_texture(0),
	m_name(""),
	m_sizex(0.0f),
	m_sizey(0.0f),
	m_id(IDProvider::getInstance().createID())
{

}

Texture::Texture(const std::string& _filepath, const std::string& _name) :
	m_filepath(_filepath),
	m_texture(0),
	m_name(_name),
	m_sizex(0.0f),
	m_sizey(0.0f),
	m_id(IDProvider::getInstance().createID())
{

}

Texture::~Texture()
{
	m_data.clear();
	deleteGLTexture();
}

bool Texture::loadData()
{
	m_data.clear();
	if (loadDDSTex(m_filepath, *this))
	{
		std::cerr << "Texture loading was successful." << std::endl;
		return true;
	}
	else
	{
		std::cerr << "Texture could not be loaded." << std::endl;
		return false;
	}
}

bool Texture::unloadData()
{
	m_data.clear();
	return true;
}

bool Texture::loadGLTexture()
{
	switch (m_type)
	{
		case TEX_2D:
			if (m_texture == 0)
			{
				if (m_data.size() == 1)	//1 face for 2D texture
				{
					if (m_data[0].size() > 0)	//at least one mipmap level
					{
						glGenTextures(1, &m_texture);
						if (m_texture == 0)
						{
							std::cerr << "gl texture object creation failed." << std::endl;
							return false;
						}
						else
						{
							glBindTexture(GL_TEXTURE_2D, m_texture);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

							for (int i = 0; i < m_data[0].size(); i++)
							{
								glCompressedTexImage2D(GL_TEXTURE_2D, m_data[0][i].getLevel(), m_data[0][i].getFormat(), m_data[0][i].getSizeX(), m_data[0][i].getSizeY(), 0, m_data[0][i].getData().size(), m_data[0][i].getData().data());
							}
							glBindTexture(GL_TEXTURE_2D, 0);

							if (glGetError() == GL_NO_ERROR)
								return true;
							else
								return false;
						}
					}
					else
					{
						std::cerr << "No texture data available." << std::endl;
						return false;
					}
				}
				else
				{
					std::cerr << "Wrong face count for 2D texture." << std::endl;
					return false;
				}				
			}
			else
			{
				std::cerr << "The texture is already bound to opengl. Call unloadGLTexture to unbind it first!" << std::endl;
				return false;
			}
			break;
		case TEX_3D:
			std::cerr << "Error. 3D textures are not supported yet." << std::endl;
			break;
		case TEX_CUBEMAP:
			std::cerr << "Error. Cubemap textures are not supported yet." << std::endl;
			return false;
			break;
		case TEX_ARRAY:
			std::cerr << "Error. Array textures are not supported yet." << std::endl;
			return false;
			break;
		case TEX_DX10_CUBEMAP:
			std::cerr << "Error. DX10 cubemap textures are not supported yet." << std::endl;
			return false;
			break;
		default:
			std::cerr << "Error. Invalid texture type." << std::endl;
			return false;
			break;
	}
	return false;
}

bool Texture::deleteGLTexture()
{

}

bool Texture::bindToTextureUnit(GLuint _unit)
{
	if (m_texture != 0)
	{
		switch (m_type)
		{
		case TEX_2D:
			glActiveTexture(_unit);
			glBindTexture(GL_TEXTURE_2D, m_texture);
			if (glGetError() == GL_NO_ERROR)
				return true;
			else
				return false;
			break;
		case TEX_3D:
			std::cerr << "Error. 3D textures are not supported yet." << std::endl;
			return false;
			break;
		case TEX_ARRAY:
			std::cerr << "Error. Array textures are not supported yet." << std::endl;
			return false;
			break;
		case TEX_CUBEMAP:
			std::cerr << "Error. Cubemap textures are not supported yet." << std::endl;
			return false;
			break;
		case TEX_DX10_CUBEMAP:
			std::cerr << "Error. DX10 cubemap textures are not supported yet." << std::endl;
			return false;
			break;
		default:
			std::cerr << "Error. Invalid texture type." << std::endl;
			return false;
			break;
		}
	}
}




