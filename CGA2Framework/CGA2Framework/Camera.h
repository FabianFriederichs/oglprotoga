#pragma once
#include "headers.h"
#include "Rotatable.h"
#include "Moveable.h"
using namespace glm;

/*
rotate with quaternions instead to circumvent gimbal lock??

default rotate values?

Direction value has to be the negative of the direction the camera points at
-abstract that and negate in constructor?
*/




class Camera
{
protected:
	vec3 m_position;
	quat m_rot;
	vec3 m_up;
	GLfloat m_fov;
	GLfloat m_width;
	GLfloat m_height;
	GLfloat m_znear;
	GLfloat m_zfar;
	
	virtual glm::mat4 Camera::Orientation() const =0;

public:

	//vec3 Direction()
	//{
	//	//return vec3(0,0,1);
	//	return glm::normalize(this->Position-Target)*-1.0f;
	//}

	//vec3 Right()
	//{
	//	return glm::normalize(glm::cross(WorldUp, Direction()));
	//}

	//vec3 Up()
	//{
	//	return WorldUp;
	//	//return glm::cross(Direction(),Right());
	//}

	virtual glm::mat4 GetViewMatrix() const =0;
	//{
	//	//	vec3 zaxis = normalize(Position - Target);    // The "forward" vector.
	//	//vec3 xaxis = normalize(cross(Up(), zaxis));// The "right" vector.
	//	//vec3 yaxis = cross(zaxis, xaxis);     // The "up" vector.
 //
	//	//// Create a 4x4 orientation matrix from the right, up, and forward vectors
	//	//// This is transposed which is equivalent to performing an inverse 
	//	//// if the matrix is orthonormalized (in this case, it is).
	//	//mat4 orientation(
	//	//   vec4( xaxis.x, yaxis.x, zaxis.x, 0 ),
	//	//   vec4( xaxis.y, yaxis.y, zaxis.y, 0 ),
	//	//   vec4( xaxis.z, yaxis.z, zaxis.z, 0 ),
	//	//   vec4(   0,       0,       0,     1 )
	//	//);
 // //   
	//	//// Create a 4x4 translation matrix.
	//	//// The eye position is negated which is equivalent
	//	//// to the inverse of the translation matrix. 
	//	//// T(v)^-1 == T(-v)
	//	//mat4 translation(
	//	//	vec4(   1,      0,      0,   0 ),
	//	//	vec4(   0,      1,      0,   0 ), 
	//	//	vec4(   0,      0,      1,   0 ),
	//	//	vec4(-Position.x, -Position.y, -Position.z, 1 )
	//	//	);
 //
	//	//// Combine the orientation and translation to compute 
	//	//// the final view matrix
	//	//return ( orientation * translation );
	//	////return lookAt(this->Position, vec3(0,0,0), vec3(0,0,0));
	//}

	virtual void Move(const MoveData &movedata) =0;
	//{
	//	vec3 oldp = this->Position;
	//	
	//	this->Position += Direction()*(movedata.Speed*movedata.Forward);
	//	this->Position += Right()*(movedata.Speed*movedata.Right);
	//	//move around target
	//	//this->Position += normalize(cross(Direction(), Up())) * (movedata.Speed*movedata.Right);

	//	//this->Target += cross(Direction(), Up()) * (movedata.Speed*movedata.Right);
	//}
	
	virtual void Rotate(const vec3 &rotation) = 0;
	//{
	//	///somehow make it work with quats
	//	rot = orientation;
	//	/*glm::vec3 target;
	//	
	//	target.x = cos(radians(rotatedata.Pitch)) * cos(radians(rotatedata.Yaw));
	//	target.y = sin(radians(rotatedata.Pitch));
	//	target.z = cos(radians(rotatedata.Pitch)) * sin(radians(rotatedata.Yaw));*/
	//	//Target=Position + (orientation*(Target-Position));
	//	//this->Direction = normalize(orientation*Direction());
	//	// Also re-calculate the Right and Up vector
	//	//this->Right = normalize(glm::cross(this->Direction, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	//	//this->Up = normalize(glm::cross(this->Right, this->Direction));
	//}

	glm::mat4 Camera::GetCameraTransform() const
	{
		return  Orientation()*translate(glm::mat4(1.0f), -m_position);
		//return  translate(glm::mat4(1.0f), -m_position)*Orientation() ;
	}

	glm::mat4 getProjectionMatrix() const
	{
		//return glm::ortho((GLfloat)-5, (GLfloat)5, (GLfloat)-5, (GLfloat)5, m_znear, m_zfar);
		return glm::perspective(m_fov, m_width / m_height, m_znear, m_zfar);
	}
};

