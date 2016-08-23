#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"

typedef enum {
	POINT,
	AXISALIGNED,
	AXIS
} BILLBOARDTYPE;

class Billboard : GameObject
{
public:
	Billboard();
	~Billboard();

private:
	Shader m_shader;
	Texture2D* m_tex;
	BILLBOARDTYPE m_type;

};

