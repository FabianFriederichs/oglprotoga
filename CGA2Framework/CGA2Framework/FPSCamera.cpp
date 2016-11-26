#include "FPSCamera.h"


FPSCamera::FPSCamera(GLfloat fov,GLint width,GLint height,GLfloat znear,GLfloat zfar, vec3 worldup, vec3 worldright, vec3 worldforward)
{
	m_wy = worldup;
	m_wx = worldright;
	m_wz = worldforward;
	getTransform().setTranslate(vec3(0, 0, 10));
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
	getTransform().setTranslate(pos);
}

glm::mat4 FPSCamera::Orientation()
{
	return mat4_cast(getTransform().getRotateQ());
}

glm::mat4 FPSCamera::GetViewMatrix() 
{
	return GetCameraTransform();
}

void FPSCamera::Fly(bool trueorwhat)
{
	flying = trueorwhat;
}

void FPSCamera::Move(const MoveData &movedata)
{

	vec3 move(0,0,0);

	if(flying)
		move += -m_transform.getForw()*movedata.mtype.x;
	else
	{
		move += normalize(cross(m_wy, m_transform.getRight()))*movedata.mtype.x;
		// move += normalize(cross(m_wy, m_wx))*movedata.mtype.x;
	}
	//move += -m_wx*movedata.mtype.z;
	move += -m_transform.getRight()*movedata.mtype.z;
	m_transform.translate(normalize(move)*movedata.Multiplier);
	if (!m_transform.isMatOverriden())
		m_transform.getTransformMat();
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
		m_transform.setRotate(normalize(m_transform.getRotateQ()*normalize(quat(vec3(0, yaw, 0)))));

	}
	if (!m_transform.isMatOverriden())
		m_transform.getTransformMat();
}