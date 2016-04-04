#pragma once
#include "headers.h"
class Texture
{
public:
	Texture();
	~Texture();

	//getters / setters
	const GLint getID() const { return m_id; }
private:
	GLint m_id;
};

