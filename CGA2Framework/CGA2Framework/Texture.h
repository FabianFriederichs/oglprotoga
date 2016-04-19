#pragma once
#include "headers.h"
#include "IDProvider.h"
#include "dds.h"

typedef enum {
	TEX_2D = 0,
	TEX_3D = 1,		//equals volume texture
	TEX_ARRAY = 2,
	TEX_CUBEMAP = 3,
	TEX_DX10_CUBEMAP = 4
} TEXTYPE;

class Texture
{
public:
	Texture();
	Texture(const Texture& _other);
	Texture(const std::string& _filepath); //single texture
	Texture(const std::string& _filepath, const std::string& _name); //single texture
	~Texture();

	bool loadData();
	bool unloadData();

	//load / unload texture to ogl
	bool loadGLTexture();
	bool bindToTextureUnit(GLuint _unit);
	bool deleteGLTexture();

	//getters / setters
	const GLint getID() const { return m_id; }
	const GLuint getGLTexture() const { return m_texture; }

	const TEXTYPE getType() const { return m_type; }
	const GLfloat getSizeX() const { return m_sizex; }
	const GLfloat getSizeY() const { return m_sizey; }

	void setSize(GLfloat _x, GLfloat _y) { this->m_sizex = _x; this->m_sizey = _y; }
	void setType(TEXTYPE _type) { this->m_type = _type; }

private:
	GLint m_id;
	
	GLuint m_texture;
	std::string m_filepath;
	std::string m_name;
	GLfloat m_sizex;
	GLfloat m_sizey;

	TEXTYPE m_type;
};
