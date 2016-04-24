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
	m_texture(0),				//a copy of a Texture object should create a separate gl texture!
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
	}
	else
	{
		std::cerr << "Texture could not be loaded." << std::endl;
	}
}

bool Texture::unloadData()
{
	m_data.clear();
}

bool Texture::loadGLTexture()
{

}

bool Texture::deleteGLTexture()
{

}

bool bindToTextureUnit(GLuint _unit)
{

}




