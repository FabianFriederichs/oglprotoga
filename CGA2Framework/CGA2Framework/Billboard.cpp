#include "Billboard.h"


Billboard::Billboard()
{
}


Billboard::~Billboard()
{
}

void Billboard::Draw()
{
	if (target!=nullptr)
		Orient();
	m_shader->Use();
	m_tex->bindToTextureUnit(0);
	m_shader->setUniform("billboard", 0);
	m_shader->setUniform("model", getTransform().getTransformMat()*rotation, false); 
	//set view and projection matrix
	m_shader->setUniform("view", m_cam->GetViewMatrix(), false);
	m_shader->setUniform("proj", m_cam->getProjectionMatrix(), false);
	
	Primitives::drawNDCQuad();
	m_tex->unbind();
}

void Billboard::Orient()
{
	auto curTrans = getTransform().getTransformMat();
	vec3 curNormal(curTrans[0][2], curTrans[1][2], curTrans[2][2]);
	auto tarNormal = m_cam->GetPosition() - getTransform().getTranslate();
	auto axis = normalize(cross(curNormal, tarNormal));
	auto angle = acos(dot(normalize(curNormal), normalize(tarNormal)));
	rotation = rotate(angle, axis);
	//Do orientation stuff to Transform with targeet
}