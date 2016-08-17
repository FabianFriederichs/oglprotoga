#include "Texture.h"

Texture::Texture(const TEXTYPE _type) :
	m_texture(0),
	m_name(""),
	m_id(IDProvider::createID()),
	m_isbound(false),
	m_isbuffered(false),
	m_type(_type)
{
	
}

Texture::Texture(const Texture& _other) :
m_path(_other.m_path),
m_name(_other.m_name),
	m_texture(_other.m_texture),
	m_id(IDProvider::createID()),	//each copy gets a new id
	m_isbound(_other.m_isbound),
	m_isbuffered(_other.m_isbuffered),
	m_type(_other.m_type)
{

}

Texture::Texture(const std::string& _name, const TEXTYPE _type) :
	m_texture(0),
	m_name(_name),
	m_id(IDProvider::createID()),
	m_isbound(false),
	m_isbuffered(false),
	m_type(_type)
{

}

Texture::~Texture()
{
	
}







