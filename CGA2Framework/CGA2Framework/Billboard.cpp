#include "Billboard.h"


Billboard::Billboard() : lockedAxes(0)
{
}


Billboard::~Billboard()
{
}

void Billboard::Draw()
{
	auto curTrans = getTransform().getTransformMat();
	vec3 curNormal(curTrans[0][2], curTrans[1][2], curTrans[2][2]);
	m_shader->Use();
	m_tex->bindToTextureUnit(0);
	m_shader->setUniform("billboard", 0);
	m_shader->setUniform("norm", curNormal);
	m_shader->setUniform("model", getTransform().getTransformMat(), false); 
	//set view and projection matrix
	m_shader->setUniform("view", m_cam->GetViewMatrix(), false);
	m_shader->setUniform("proj", m_cam->getProjectionMatrix(), false);
	
	Primitives::drawNDCQuad();
	m_tex->unbind();
}

void Billboard::Orient()
{
	auto pos = getTransform().getTranslate();
	auto tar = normalize(target->getTransform().getTranslate()-pos);
	vec3 tUp(0);
	if (length(lockedAxes) == 0)
	{
		tUp = target->getTransform().getUp();
	}
	else
	{
		tUp = lockedAxes;
	}
	auto right = normalize(cross(tUp, tar));
	if (lockedAxes.length() != 0)
	{
		tar = normalize(cross(right, tUp));
	}
	auto up = cross(tar, right);
	mat4 trans(vec4(right.x, right.y, right.z, 0), vec4(up.x, up.y, up.z, 0), vec4(tar.x, tar.y, tar.z, 0), vec4(pos[0], pos[1], pos[2], 1));
	//auto angle = acos(dot(normalize(curNormal), normalize(tarNormal)));
	getTransform().setRotate(normalize(glm::quat(trans)));
	//Do orientation stuff to Transform with targeet
}

void Billboard::lockAxis(vec3 _ax)
{
	if (_ax.x != 0)
		_ax.x = 1;
	if (_ax.y != 0)
		_ax.y = 1;
	if (_ax.z != 0)
		_ax.z = 1;
	lockedAxes = _ax;
}