#include "Material.h"

Material::Material() :	//member initializers
	m_id(IDProvider::createID()),
	m_ambientcolor(0.0f, 0.0f, 0.0f, 0.0f),
	m_diffusecolor(0.0f, 0.0f, 0.0f, 0.0f),
	m_specularcolor(0.0f, 0.0f, 0.0f, 0.0f),
	m_name(""),
	m_shininess(0.0f),
	m_alpha(1.0f),
	m_textures(),
	m_shader(nullptr)
{
	
}

Material::Material(const std::string& _name,
	const glm::vec4& _ambientcolor,
	const glm::vec4& _diffusecolor,
	const glm::vec4& _specularcolor,
	const GLfloat _shininess,
	const GLfloat _alpha) :	//member initializers
	m_id(IDProvider::createID()),
	m_ambientcolor(_ambientcolor),
	m_diffusecolor(_diffusecolor),
	m_specularcolor(_specularcolor),
	m_name(_name),
	m_shininess(_shininess),
	m_alpha(_alpha),
	m_textures(),
	m_shader(nullptr)

{
	
}

Material::Material(const std::string& _name,
	const std::vector<Texture2D*>& _textures,
	const glm::vec4& _ambientcolor,
	const glm::vec4& _diffusecolor,
	const glm::vec4& _specularcolor,
	const GLfloat _shininess,
	const GLfloat _alpha,
	Shader* _shader) :	//member initializers
	m_id(IDProvider::createID()),
	m_ambientcolor(_ambientcolor),
	m_diffusecolor(_diffusecolor),
	m_specularcolor(_specularcolor),
	m_name(_name),
	m_shininess(_shininess),
	m_alpha(_alpha),
	m_textures(_textures),
	m_shader(_shader)
{
	
}
//copy constructor
Material::Material(const Material& _other) : //member initializers
	m_id(IDProvider::createID()),
	m_ambientcolor(_other.m_ambientcolor),
	m_diffusecolor(_other.m_diffusecolor),
	m_specularcolor(_other.m_specularcolor),
	m_name(_other.m_name),
	m_shininess(_other.m_shininess),
	m_alpha(_other.m_alpha),
	m_textures(_other.m_textures),
	m_shader(_other.m_shader)
{
	
}



Material::~Material()
{

}

void Material::addTexture(Texture2D* _texture)
{
	m_textures.push_back(_texture);
}

void Material::removeTexture(const GLint _id)
{
	m_textures.erase(std::remove_if(m_textures.begin(), m_textures.end(), [_id](const Texture2D* x)
	{
		return x->getID() == _id;
	}), m_textures.end());
}

GLint Material::getTextureCount()
{
	return m_textures.size();
}

Texture2D* Material::getTexture(const GLint _id)
{
	for (std::vector<Texture2D*>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		if ((*it)->getID() == _id)
			return (*it);
	}
	return nullptr; //dummy
}

//void Material::fillShaderUniforms(Shader* _shader)
//{
//	_shader->setUniform("material.ambientcolor", m_ambientcolor);
//	_shader->setUniform("material.diffusecolor", m_diffusecolor);
//	_shader->setUniform("material.specularcolor", m_specularcolor);
//	_shader->setUniform("material.shininess", m_shininess);
//	_shader->setUniform("material.alpha", m_alpha);
//
//	//textures
//	for (GLint i = 0; i < m_textures.size() ; i++)
//	{
//		_shader->setUniform("material.mtex["+ std::to_string(i) + "]", i);
//		m_textures[i]->bindToTextureUnit(i);
//	}
//
//	_shader->setUniform("material.texcount", (GLint)m_textures.size());
//
//}

void Material::setMaterialUniforms()
{
	m_shader->setUniform("material.ambient", this->getAmbientColor());
	m_shader->setUniform("material.diffuse", this->getDiffuseColor());
	m_shader->setUniform("material.specular", this->getSpecularColor());
	m_shader->setUniform("material.shininess", this->getShininess());
	m_shader->setUniform("material.alpha", this->getAlpha());

	//textures
	for (GLint i = 0; i < this->getTextures().size(); i++)
	{
		/*if (!this->getTextures()[i]->isLoaded())
		{
			if (!this->getTextures()[i]->loadData())
			{
				std::cerr << "ERROR: Texture could not be loaded from file.\n";
			}
		}*/
		if (!this->getTextures()[i]->isBuffered())
		{
			if (!this->getTextures()[i]->buffer(false))
			{
				std::cerr << "ERROR: Texture could not be loaded by OpenGL.\n";
			}
		}
		/*if (!this->getTextures()[i]->isBound())
		{*/
			if (!this->getTextures()[i]->bindToTextureUnit(i))
			{
				std::cerr << "ERROR: Texture could not be bound to texture unit";
			}
		//}

		m_shader->setUniform("material.mtex[" + std::to_string(i) + "]", i);
		//_material->getTextures()[i]->bindToTextureUnit(i);
	}

	m_shader->setUniform("material.texcount", (GLint)(this->getTextures().size()));
}
