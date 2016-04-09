#include "Texture.h"

Texture::Texture() :
	m_filepath(),
	m_texture(0),
	m_name(),
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
}

void Texture::loadGLTexture()
{

}

void Texture::deleteGLTexture()
{
}
