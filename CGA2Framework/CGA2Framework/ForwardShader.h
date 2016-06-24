#pragma once
#include "Shader.h"
#include "headers.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
class ForwardShader :
	public Shader
{
public:
	ForwardShader();
	virtual ~ForwardShader();

	const GLint MAX_DIR_LIGHTS = 8;
	const GLint MAX_POINT_LIGHTS = 16;
	const GLint MAX_SPOT_LIGHTS = 16;



	void setDirectionalLights(std::vector<DirectionalLight*>* dirlights){};
	void setPointLights(std::vector<PointLight*>* dirlights){};
	void setSpotLights(std::vector<SpotLight*>* dirlights){};

	virtual void preRenderActions();
	virtual void postRenderActions();
};

