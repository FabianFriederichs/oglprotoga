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




class Camera: Moveable
{
public:
	Camera(const vec3 position = vec3(0.0f, 0.0f, 3.0f), const vec3 up = vec3(0.0f, 1.0f, 0.0f), const vec3 target = vec3(0.0f, 0.0f, 1.0f))
	{
		this->Position = position;
		this->WorldUp = up;
		Target = target;
		vec3 d = Direction();
		//this->Direction = glm::normalize(this->Position - target)*-1.0f;
	}
	~Camera();

	vec3 Position;
	vec3 Target;
	vec3 WorldUp;

	vec3 Direction()
	{
		return glm::normalize(this->Position - Target)*-1.0f;
	}

	vec3 Right()
	{
		return glm::normalize(glm::cross(WorldUp, Direction()));
	}

	vec3 Up()
	{
		return glm::cross(Direction(),Right());
	}

	glm::mat4 GetViewMatrix()
	{
		return lookAt(this->Position, Direction(), Up());
	}

	void Move(const MoveData &movedata)
	{
		vec3 oldp = this->Position;
		
		this->Position += Direction()*(movedata.Speed*movedata.Forward);
		this->Position += Right() *(movedata.Speed*movedata.Right);
		//move around target
		//this->Position += normalize(cross(Direction(), Up())) * (movedata.Speed*movedata.Right);

		//this->Target += cross(Direction(), Up()) * (movedata.Speed*movedata.Right);
	}

	void Rotate(const quat &orientation)
	{
		///somehow make it work with quats
		/*glm::vec3 target;
		target.x = cos(radians(rotatedata.Pitch)) * cos(radians(rotatedata.Yaw));
		target.y = sin(radians(rotatedata.Pitch));
		target.z = cos(radians(rotatedata.Pitch)) * sin(radians(rotatedata.Yaw));*/
		Target=Position + (orientation*(Target-Position));
		//this->Direction = normalize(orientation*Direction());
		// Also re-calculate the Right and Up vector
		//this->Right = normalize(glm::cross(this->Direction, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		//this->Up = normalize(glm::cross(this->Right, this->Direction));
	}
};

