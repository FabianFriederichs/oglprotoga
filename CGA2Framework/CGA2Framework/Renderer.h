#pragma once
#include "headers.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Mesh.h"
#include "RenderableGameObject.h"
#include "Camera.h"
#include "ForwardShader.h"

typedef struct meshcmp {
	bool operator()(Mesh*& m1, Mesh*& m2) const
	{
		return m1->getMaterial()->getID() > m2->getMaterial()->getID();
	}
};
typedef std::pair<Mesh*, RenderableGameObject*> RenderPair;
typedef std::list<std::pair<Mesh*, RenderableGameObject*>> RenderList;

class Renderer
{
private:
	void setLight();
public:
	Renderer();
	~Renderer();

	void rendermultipass(RenderList& _renderlist,
		Camera* _camera,
		const std::list<DirectionalLight*>& _dirlights,
		const std::list<PointLight*>& _pointlights,
		const std::list<SpotLight*>& _spotlights);

	void renderforward(RenderList& _renderlist,
		Camera* _camera,
		const std::list<DirectionalLight*>& _dirlights,
		const std::list<PointLight*>& _pointlights,
		const std::list<SpotLight*>& _spotlights);
};

