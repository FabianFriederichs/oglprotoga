#include "ForwardShader.h"


ForwardShader::ForwardShader()
{
}


ForwardShader::~ForwardShader()
{
}

void ForwardShader::preRenderActions()
{
	//glEnable(GL_DEPTH_TEST);
}

void ForwardShader::postRenderActions()
{
	//glDisable(GL_DEPTH_TEST);
}

void ForwardShader::setModelMatrix(glm::mat4& _mat)
{
	this->m_modelmat = _mat;
	this->setUniform("model", _mat, false);
}

void ForwardShader::setViewMatrix(glm::mat4& _mat)
{
	this->m_viewmat = _mat;
	this->setUniform("view", _mat, false);
}

void ForwardShader::setProjectionMatrix(glm::mat4& _mat)
{
	this->m_projectionmat = _mat;
	this->setUniform("projection", _mat, false);
}

void ForwardShader::setLights(
	const std::list<DirectionalLight*>& dirLights,
	const std::list<PointLight*>& pointLights,
	const std::list<SpotLight*>& spotLights)
{
	GLint ct = 0;
	//set directional light uniforms
	for (std::list<DirectionalLight*>::const_iterator it = dirLights.begin(); it != dirLights.end(); it++)
	{
		if (ct + 1 > MAX_DIR_LIGHTS)
			break;
		this->setUniform("dirlights[" + std::to_string(ct) + "].lightdir",  (*it)->m_direction);
		this->setUniform("dirlights[" + std::to_string(ct) + "].lightcol", (*it)->m_lightcolor);		
		ct++;
	}
	this->setUniform("dirlightcount", ct);

	ct = 0;
	//set pointlight uniforms
	for (std::list<PointLight*>::const_iterator it = pointLights.begin(); it != pointLights.end(); it++)
	{
		if (ct + 1 > MAX_DIR_LIGHTS)
			break;
		this->setUniform("pointlights[" + std::to_string(ct) + "].lightpos", (*it)->getTransform().getTranslate());
		this->setUniform("pointlights[" + std::to_string(ct) + "].lightcol", (*it)->m_lightcolor);
		this->setUniform("pointlights[" + std::to_string(ct) + "].cterm", (*it)->m_constant);
		this->setUniform("pointlights[" + std::to_string(ct) + "].lterm", (*it)->m_linear);
		this->setUniform("pointlights[" + std::to_string(ct) + "].qterm", (*it)->m_quadratic);
		this->setUniform("pointlights[" + std::to_string(ct) + "].range", (*it)->m_range);
		ct++;
	}
	this->setUniform("pointlightcount", ct);

	ct = 0;
	//set spotlight uniforms
	for (std::list<SpotLight*>::const_iterator it = spotLights.begin(); it != spotLights.end(); it++)
	{
		if (ct + 1 > MAX_DIR_LIGHTS)
			break;
		this->setUniform("spotlights[" + std::to_string(ct) + "].lightpos", (*it)->getTransform().getTranslate());
		this->setUniform("spotlights[" + std::to_string(ct) + "].lightcol", (*it)->m_lightcolor);
		this->setUniform("spotlights[" + std::to_string(ct) + "].lightdir", (*it)->m_direction);
		this->setUniform("spotlights[" + std::to_string(ct) + "].cterm", (*it)->m_constant);
		this->setUniform("spotlights[" + std::to_string(ct) + "].lterm", (*it)->m_linear);
		this->setUniform("spotlights[" + std::to_string(ct) + "].qterm", (*it)->m_quadratic);
		this->setUniform("spotlights[" + std::to_string(ct) + "].innercone", (*it)->m_cosinnercone);
		this->setUniform("spotlights[" + std::to_string(ct) + "].qterm", (*it)->m_cosoutercone);
		this->setUniform("spotlights[" + std::to_string(ct) + "].range", (*it)->m_range);
		ct++;
	}
	this->setUniform("spotlightcount", ct);

}

void ForwardShader::setCameraPos(glm::vec3 _position)
{
	this->setUniform("camerapos", _position);
}