#pragma once
#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Primitives.h"
typedef enum {
	POINT,
	AXISALIGNED,
	AXIS
} BILLBOARDTYPE;

class Billboard : public GameObject
{
public:
	Billboard();
	~Billboard();

	void Draw();
	void setTexture(Texture2D* _tex)
	{
		m_tex = _tex;
	}
	void setCamera(Camera* _cam){ m_cam = _cam; }
	void setShader(Shader* _shader){ m_shader = _shader; }
	void setTarget(GameObject*_go){ target = _go; }
private:
	Shader* m_shader;
	Texture2D* m_tex;
	BILLBOARDTYPE m_type;
	Camera* m_cam;
	GameObject* target;
	void Orient();
};

