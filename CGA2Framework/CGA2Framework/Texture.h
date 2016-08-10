#pragma once
#include "headers.h"
#include "IDProvider.h"
#include "glerror.h"

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
	Texture(const TEXTYPE _type);
	Texture(const Texture& _other); //
	Texture(const std::string& _name, const TEXTYPE _type); //single file texture
	virtual ~Texture();

	//Buffering and binding
	virtual bool buffer() = 0;	//created gl texture can be accessed via getGLTexture()
	virtual bool unbuffer() = 0;
	virtual bool bind() = 0;
	virtual bool bindToTextureUnit(const GLuint _unit) = 0;
	virtual bool unbind() = 0;
	const GLuint getGLTexture() const { return m_texture; }

	//getters / setters
	void setType(TEXTYPE _type) { this->m_type = _type; }
	const TEXTYPE getType() const { return m_type; }
	const GLint getID() const { return m_id; }
	const bool isBuffered() { return m_isbuffered; }
	const bool isBound() { return m_isbound; }

protected:
	GLint m_id;	
	GLuint m_texture;
	std::string m_name;	
	TEXTYPE m_type;	
	bool m_isbuffered;
	bool m_isbound;
};
