#include "Billboard.h"


Billboard::Billboard()
{
}


Billboard::~Billboard()
{
}

void Billboard::Draw(void* _target)
{
	target = _target;
	Orient();
	m_shader->Use();
	m_tex->bindToTextureUnit(0);
	m_shader->setUniform("billboard", 0);
	m_shader->setUniform("model", getTransform().getTransformMat(), false); 
	//set view and projection matrix
	m_shader->setUniform("view", m_cam->GetViewMatrix(), false);
	m_shader->setUniform("proj", m_cam->getProjectionMatrix(), false);
	
	Primitives::drawNDCQuad();
	m_tex->unbind();
}

void Billboard::Orient()
{
	//Do orientation stuff to Transform with targeet
}