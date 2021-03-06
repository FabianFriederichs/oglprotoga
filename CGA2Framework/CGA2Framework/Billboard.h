#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Primitives.h"
#include "RenderableGameObject.h"

class Billboard : public RenderableGameObject
{
public:
	Billboard() :lockedAxes(0){ setModel(new Model(PRIMITIVETYPE::QUAD)); };
	~Billboard();

	void DrawDeferred()
	{
		auto curTrans = getTransform().getTransformMat();

		Primitives::drawNDCQuad();
	}
	void Draw();
	void setTexture(Texture2D* _tex)
	{
		m_tex = _tex;
	}
	void setCamera(Camera* _cam){ m_cam = _cam; }
	void setShader(Shader* _shader){ m_shader = _shader; }
	void setTarget(GameObject*_go){ target = _go; }
	void lockAxis(vec3 _ax);
	void Orient();
private:
	Shader* m_shader;
	Texture2D* m_tex;
	Camera* m_cam;
	GameObject* target;
	vec3 lockedAxes;
};

