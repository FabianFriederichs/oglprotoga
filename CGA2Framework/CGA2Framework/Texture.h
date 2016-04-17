#pragma once
#include "headers.h"
#include "IDProvider.h"

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
	Texture(const std::string& _filepath, const std::string& _name); //single texture
	~Texture();

	//factory method
	//static Texture loadTexFromDDS(const std::string& _filepath);

	//load / unload texture to ogl
	void loadGLTexture();
	void bindToTextureUnit(GLuint _unit);
	void deleteGLTexture();

	//getters / setters
	const GLint getID() const { return m_id; }
	const GLuint getGLTexture() const { return m_texture; }

	const TEXTYPE getType() const { return m_type; }
	const DWORD getFormat() const { return m_format; }
	const GLfloat getSizeX() const { return m_sizex; }
	const GLfloat getSizeY() const { return m_sizey; }

	void setSize(GLfloat _x, GLfloat _y) { this->m_sizex = _x; this->m_sizey = _y; }
	void setType(TEXTYPE _type) { this->m_type = _type; }
	void setFormat(DWORD _format) { this->m_format = _format; }
	std::vector<std::vector<std::vector<unsigned char>>>& getData() { return m_data; }
	void setData(std::vector<std::vector<std::vector<unsigned char>>>& _data) { m_data.clear(); m_data = _data; }

private:
	GLint m_id;
	
	GLuint m_texture;
	std::string m_filepath;
	std::string m_name;
	GLfloat m_sizex;
	GLfloat m_sizey;

	TEXTYPE m_type;
	DWORD m_format;
	std::vector<std::vector<std::vector<unsigned char>>> m_data;	//surface -> mipmap -> mipmapdata
};
