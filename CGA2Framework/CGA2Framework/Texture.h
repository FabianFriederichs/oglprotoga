#pragma once
#include "headers.h"
#include "IDProvider.h"

class Texture
{
public:
	Texture();
	Texture(const Texture& _other);
	Texture(const std::string& _filepath, const std::string& _name); //single texture
	~Texture();

	//load / unload texture to ogl
	void loadGLTexture();
	void deleteGLTexture();

	//getters / setters
	const GLint getID() const { return m_id; }
	const GLuint getGLTexture() const { return m_texture; }

private:
	GLint m_id;
	
	GLuint m_texture;
	std::string m_filepath;
	std::string m_name;
	GLfloat m_sizex;
	GLfloat m_sizey;	
};
