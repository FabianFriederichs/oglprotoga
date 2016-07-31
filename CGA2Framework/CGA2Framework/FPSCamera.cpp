#include "FPSCamera.h"


FPSCamera::FPSCamera(GLfloat fov,GLint width,GLint height,GLfloat znear,GLfloat zfar, vec3 up)
{
	m_up = up;
	m_position=vec3(0,0,10);
	m_rot = quat(vec3(0.0f, 0.0f, 0.0f));
	m_fov = fov;
	m_height = height;
	m_width = width;
	m_znear = znear;
	m_zfar = zfar;
	m_VA = 0;
	m_HA = 0;
}


//FPSCamera::~FPSCamera(void)
//{
//
//}

glm::mat4 FPSCamera::Orientation() const
{
	//return mat4_cast(m_rot);
	return mat4_cast(normalize(m_rot));
}

glm::mat4 FPSCamera::GetViewMatrix() const
{
	//auto rotation = mat4_cast(quat(vec3(0,yaw(m_rot), 0)));
	//auto dir = normalize(m_position+vec3(rotation*vec4(0,1,0,1)));
	////auto dir = normalize(m_position+vec3(Orientation()*vec4(1,1,0,1)));
	//return lookAt(m_position, dir, m_up);
	return GetCameraTransform();
}

void FPSCamera::Move(const MoveData &movedata)
{
	
	auto rotation = mat4_cast(quat(vec3(pitch(m_rot),yaw(m_rot), 0)));
	auto dir = normalize(m_position+vec3(rotation*vec4(1,1,0,1)));
	//auto r = normalize(vec3(rotation*vec4(movedata.asVec3()*-1.0f,1.0)));
	std::cout << dir.x <<" " <<dir.y <<" "<< dir.z <<std::endl;
	vec3 move(0,0,0);
	/*if(HasFlag(movedata.mtype, MovementType::FORWARD))
	{
		move-=dir;
	}
	if(HasFlag(movedata.mtype, MovementType::BACKWARD))
	{
		move+=dir;
	}
	if(HasFlag(movedata.mtype, MovementType::STRAFE_L))
	{
		move+= glm::cross(dir, m_up);
	}
	if(HasFlag(movedata.mtype, MovementType::STRAFE_R))
	{
		move-= glm::cross(dir, m_up);
	}*/

	move+=dir*movedata.mtype.x*-1.0f;
	move+=cross(dir, m_up)*movedata.mtype.z;
	m_position+= move*movedata.Multiplier;
}

void FPSCamera::Rotate(const vec3 &rotation)
{
	m_VA = rotation.y+m_VA;
	m_HA = rotation.x+m_HA;
	if(m_VA>90)
		m_VA=90;
	if(m_VA<-90)
		m_VA = -90;
	if(m_HA<0)
		m_HA=360+m_HA;
	if(m_HA>=360)
		m_HA-=360;
	//std::cout << m_HA<<" HA " <<std::endl;
	m_rot=normalize(quat(vec3(radians(m_VA), radians(m_HA), 0)));
}