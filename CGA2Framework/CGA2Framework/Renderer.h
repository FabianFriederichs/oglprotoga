#pragma once
#include "headers.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Mesh.h"
#include "RenderableGameObject.h"
#include "Camera.h"

typedef struct meshcmp {
	bool operator()(const Mesh*& m1, const Mesh*& m2) const
	{
		return m1->getShader()->getID() > m2->getShader()->getID();
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
		std::vector<DirectionalLight*>* _dirlights = nullptr,
		std::vector<PointLight*>* _pointlights = nullptr,
		std::vector<SpotLight*>* _spotlights = nullptr);

	void renderforward(RenderList& _renderlist,
		Camera* _camera,
		std::vector<DirectionalLight*>* _dirlights = nullptr,
		std::vector<PointLight*>* _pointlights = nullptr,
		std::vector<SpotLight*>* _spotlights = nullptr);
};

