#pragma once
#include "headers.h"
#include "Rotatable.h"
#include "Moveable.h"
using namespace glm;

/*
rotate with quaternions instead to circumvent gimbal lock??

default rotate values?
*/




class Camera: Rotatable, Moveable
{
public:
	Camera(const GLfloat width, const GLfloat height, const GLfloat znear, const GLfloat zfar, const GLfloat fov, const vec3 position = vec3(0.0f, 0.0f, 3.0f), const vec3 up = vec3(0.0f, 1.0f, 0.0f), const vec3 target = vec3(0.0f, 0.0f, 0.0f))
	{
		this->Position = position;
		this->WorldUp = up;
		this->Direction = glm::normalize(this->Position - target)*-1.0f;
		this->Right = glm::normalize(glm::cross(up, this->Direction));
		this->Up = glm::cross(this->Direction, this->Right);
		this->width = width;
		this->height = height;
		this->znear = znear;
		this->zfar = zfar;
		this->fov = fov;
	}
	~Camera();

	vec3 Position;
	vec3 Direction;
	vec3 Up;
	vec3 WorldUp;
	vec3 Right;
	float width;
	float height;
	float znear;
	float zfar;
	float fov;

	glm::mat4 GetViewMatrix()
	{
		return lookAt(this->Position, this->Position + this->Direction, this->Up);
	}


	void Move(const MoveData &movedata)
	{
		this->Position += (movedata.Speed*movedata.Forward) * this->Direction;
		this->Position -= (movedata.Speed*movedata.Backward)  * this->Direction;
		this->Position -= normalize(cross(this->Direction, Up)) * (movedata.Speed*movedata.Left);
		this->Position += normalize(cross(this->Direction, Up)) * (movedata.Speed*movedata.Right);
	}

	void Rotate(const RotateData &rotatedata)
	{
		glm::vec3 target;
		target.x = cos(radians(rotatedata.Pitch)) * cos(radians(rotatedata.Yaw));
		target.y = sin(radians(rotatedata.Pitch));
		target.z = cos(radians(rotatedata.Pitch)) * sin(radians(rotatedata.Yaw));
		
		this->Direction = normalize(target);
		// Also re-calculate the Right and Up vector
		this->Right = normalize(glm::cross(this->Direction, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = normalize(glm::cross(this->Right, this->Direction));
	}

	glm::mat4 getProjectionMatrix()
	{
		glm::perspective(fov, (GLfloat)width / (GLfloat)height, znear, zfar);
	}
};

