#include "FPSCamera.h"


FPSCamera::FPSCamera(GLfloat fov,GLint width,GLint height,GLfloat znear,GLfloat zfar, vec3 worldup, vec3 worldright, vec3 worldforward)
{
	m_wy = worldup;
	m_wx = worldright;
	m_wz = worldforward;
	m_transform.setTranslate(vec3(0, 0, 10));
	m_transform.setRotate(quat(vec3(0.0f, 0.0f, 0.0f)));
	m_fov = fov;
	m_height = height;
	m_width = width;
	m_znear = znear;
	m_zfar = zfar;
	m_VA = 0;
	m_HA = 0;
	flying = false;
	recalcProj();
}

void FPSCamera::SetPosition(vec3 pos)
{
	 m_transform.setTranslate(pos);
}

glm::mat4 FPSCamera::Orientation() const
{
	return mat4_cast(m_transform.getRotateQ());
}

glm::mat4 FPSCamera::GetViewMatrix() const
{
	return GetCameraTransform();
}

void FPSCamera::Fly(bool trueorwhat)
{
	flying = trueorwhat;
}

void FPSCamera::Move(const MoveData &movedata)
{
	auto ori = Orientation();
	vec3 zaxis(ori[0][2], ori[1][2], ori[2][2]);
	vec3 xaxis(ori[0][0], ori[1][0], ori[2][0]);
	auto dir = -zaxis;

	vec3 move(0,0,0);

	if(flying)
	move+=dir*movedata.mtype.x;
	else
	{
		move+=normalize(cross(m_wy, xaxis))*movedata.mtype.x;
	}
	move+=-xaxis*movedata.mtype.z;
	m_transform.translate(move*movedata.Multiplier);
}

void FPSCamera::Rotate(const vec3 &rotation)
{
	m_VA = degrees(rotation.y)+m_VA;

	GLfloat pitch = rotation.y;
	GLfloat yaw = rotation.x;

	if(m_VA>89)
	{
		m_VA=89;
		pitch= 0;
	}
	else if(m_VA<-89)
	{
		m_VA = -89;
		pitch = 0;
	}

	if(pitch!=0)
	{
		m_transform.setRotate(normalize(quat(vec3(pitch, 0, 0))*m_transform.getRotateQ()));
	}

	if(yaw!=0)
	{
		m_transform.setRotate(normalize(m_transform.getRotateQ()*quat(vec3(0, yaw, 0))));

	}
}