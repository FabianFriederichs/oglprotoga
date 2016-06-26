#pragma once
#include "Shader.h"
#include "headers.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#define MAX_DIR_LIGHTS 8
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16
#define MAX_TEXTURES 8
class ForwardShader :
	public Shader
{
public:
	ForwardShader();
	virtual ~ForwardShader();

	void setModelMatrix(glm::mat4& _mat);
	void setViewMatrix(glm::mat4& _mat);
	void setProjectionMatrix(glm::mat4& _mat);

	glm::mat4& getModelMatrix() { return m_modelmat; }
	glm::mat4& getViewMat() { return m_viewmat; }
	glm::mat4& getProjectionMat() { return m_projectionmat; }

	/*void setMaterialUniforms(Material* _material);*/

	void setLights(const std::list<DirectionalLight*>& dirlights, const std::list<PointLight*>& pointlights, const std::list<SpotLight*>& spotlights);
	
	virtual void preRenderActions();
	virtual void postRenderActions();

private:
	glm::mat4 m_viewmat;
	glm::mat4 m_modelmat;
	glm::mat4 m_projectionmat;
};

