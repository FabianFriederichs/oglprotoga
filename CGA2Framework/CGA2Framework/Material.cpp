#include "Material.h"

Material::Material() :	//member initializers
	m_id(IDProvider::getInstance().createID()),
	m_ambientcolor(0.0f, 0.0f, 0.0f, 0.0f),
	m_diffusecolor(0.0f, 0.0f, 0.0f, 0.0f),
	m_specularcolor(0.0f, 0.0f, 0.0f, 0.0f),
	m_name(""),
	m_shininess(0.0f),
	m_alpha(1.0f),
	m_textures()
{
	
}

Material::Material(const std::string& _name,
	const glm::vec4& _ambientcolor,
	const glm::vec4& _diffusecolor,
	const glm::vec4& _specularcolor,
	const GLfloat _shininess,
	const GLfloat _alpha) :	//member initializers
	m_id(IDProvider::getInstance().createID()),
	m_ambientcolor(_ambientcolor),
	m_diffusecolor(_diffusecolor),
	m_specularcolor(_specularcolor),
	m_name(_name),
	m_shininess(_shininess),
	m_alpha(_alpha),
	m_textures()

{
	
}

Material::Material(const std::string& _name,
	const std::vector<Texture>& _textures,
	const glm::vec4& _ambientcolor,
	const glm::vec4& _diffusecolor,
	const glm::vec4& _specularcolor,
	const GLfloat _shininess,
	const GLfloat _alpha) :	//member initializers
	m_id(IDProvider::getInstance().createID()),
	m_ambientcolor(_ambientcolor),
	m_diffusecolor(_diffusecolor),
	m_specularcolor(_specularcolor),
	m_name(_name),
	m_shininess(_shininess),
	m_alpha(_alpha),
	m_textures(_textures)
{
	
}
//copy constructor
Material::Material(const Material& _other) : //member initializers
	m_id(IDProvider::getInstance().createID()),
	m_ambientcolor(_other.m_ambientcolor),
	m_diffusecolor(_other.m_diffusecolor),
	m_specularcolor(_other.m_specularcolor),
	m_name(_other.m_name),
	m_shininess(_other.m_shininess),
	m_alpha(_other.m_alpha),
	m_textures(_other.m_textures)
{
	
}



Material::~Material()
{

}

void Material::addTexture(const Texture& _texture)
{
	m_textures.push_back(_texture);
}

void Material::removeTexture(const GLint _id)
{
	m_textures.erase(std::remove_if(m_textures.begin(), m_textures.end(), [&_id](const Texture& x){return x.getID() == _id; }), m_textures.end());
}

GLint Material::getTextureCount()
{
	return m_textures.size();
}

Texture& Material::getTexture(const int& _id)
{
	for (std::vector<Texture>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		if (it->getID() == _id)
			return (*it);
	}
	return Texture(); //dummy
}

void Material::applyMaterialUniforms(Shader& _shader)
{
	
}
