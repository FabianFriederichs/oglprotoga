#pragma once
#include "headers.h"
class Texture
{
public:
	Texture();
	~Texture();

	//getters / setters
	const GLint getID() const { return m_id; }

	//enums and stuff
	//enums and stuff
	enum class TEXTYPE
	{
		TEX_DIFFUSE = 0,
		TEX_SPECULAR = 1,
		TEX_NORMAL = 2,
		TEX_HEIGHT = 3
	};

private:
	GLint m_id;
	TEXTYPE m_textype;
};

