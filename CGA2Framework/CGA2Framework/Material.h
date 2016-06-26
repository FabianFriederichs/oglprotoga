#pragma once
#include "headers.h"
#include "Texture.h"
#include "IDProvider.h"
#include "Shader.h"
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
		const std::vector<Texture*>& _textures,
		const glm::vec4& _ambientcolor,
		const glm::vec4& _diffusecolor,
		const glm::vec4& _specularcolor,
		const GLfloat _shininess,
		const GLfloat _alpha,
		Shader* _shader);

	Material(const Material& _other);	//copy

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

	Shader* getShader() const { return m_shader; }
	void setShader(Shader* _shader) { m_shader = _shader; }

	//Textures
	void removeTexture(const GLint _id);
	void addTexture(Texture* _texture);
	std::vector<Texture*>& getTextures() { return m_textures; }
	GLint getTextureCount();
	Texture* getTexture(const int _id);

	void setMaterialUniforms();

private:
	GLint m_id;							//each material has a unique id
	std::string m_name;
	std::vector<Texture*> m_textures;  //i.e. <diffuse_map, specular_map, normal_map, height_map> -> holds all data for standard shading, normal mapping, bump mapping, parallax mapping (normalmap + heightmap), displacement mapping
	//standard phong stuff
	glm::vec4 m_ambientcolor;			//if no diffuse_map given
	glm::vec4 m_diffusecolor;			//if no diffuse_map given
	glm::vec4 m_specularcolor;			//if no specular_map given
	GLfloat m_shininess;				//for phong shading, range [0.0, 1.0], higher values -> smaller specular
	GLfloat m_alpha;					//some value from obj, don't know what this is
	Shader* m_shader;
};

