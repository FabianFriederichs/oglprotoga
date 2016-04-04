#pragma once
#include "headers.h"
#include "Texture.h"
#include "IDProvider.h"
class Material
{
public:
	Material();

	Material(const std::string& _name,
		const glm::vec4& _ambientcolor,
		const glm::vec4& _diffusecolor,
		const glm::vec4& _specularcolor,
		const GLfloat _shininess,
		const GLfloat _alpha);

	Material(const std::string& _name,
		const std::vector<Texture>& _textures,
		const glm::vec4& _ambientcolor,
		const glm::vec4& _diffusecolor,
		const glm::vec4& _specularcolor,
		const GLfloat _shininess,
		const GLfloat _alpha);

	Material(const Material& _other);

	~Material();

	//getters / setters
	const GLint getID() const {return m_id;}
	const std::string& getName() const { return m_name; }
	const glm::vec4& getAmbientColor() const { return m_ambientcolor; }
	const glm::vec4& getDiffuseColor() const { return m_diffusecolor; }
	const glm::vec4& getSpecularColor() const { return m_specularcolor; }
	const GLfloat getShininess() const { return m_shininess; }
	const GLfloat getAlpha() const { return m_alpha; }

	void setName(const std::string& _name) { m_name = _name; }
	void setAmbientColor(const glm::vec4& _ambientcolor) { m_ambientcolor = _ambientcolor; }
	void setDiffuseColor(const glm::vec4& _diffusecolor) { m_diffusecolor = _diffusecolor; }
	void setSpecularColor(const glm::vec4& _specularcolor) { m_specularcolor = _specularcolor; }
	void setShininess(const GLfloat _shininess) { m_shininess = _shininess; }
	void setAlpha(const GLfloat _alpha) { m_alpha = _alpha; }

	//Textures
	void removeTexture(const GLint _id);
	void addTexture(const Texture& _texture);
	void addTexture(const Texture&& _texture);
	GLint getTextureCount();
	Texture& getTexture(const int& _id);

private:
	GLint m_id;							//each material has a unique id
	std::string m_name;
	std::vector<Texture> m_textures;
	//standard phong stuff
	glm::vec4 m_ambientcolor;
	glm::vec4 m_diffusecolor;
	glm::vec4 m_specularcolor;
	GLfloat m_shininess;
	GLfloat m_alpha;
};

