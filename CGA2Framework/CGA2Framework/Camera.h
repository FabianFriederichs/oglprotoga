#pragma once
#include "headers.h"
using namespace glm;

/*
Moveable interface
Rotatable interface
RotateData
Move *Data to *able
Inherit from it

rotate with quaternions instead to circumvent gimbal lock??

default rotate values?
*/
struct MoveData
{
	GLfloat Speed;
	GLfloat Forward;
	GLfloat Backward;
	GLfloat Left;
	GLfloat Right;
};

struct RotateData
{
	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Roll;
};

class Camera
{
public:
	Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), vec3 target = vec3(0.0f, 0.0f, -1.0f))
	{
		this->Position = position;
		this->WorldUp = up;
		this->Target = target;
	}
	~Camera();

	glm::vec3 Position;
	glm::vec3 Target;
	glm::vec3 Direction;
	vec3 Up;
	vec3 WorldUp;
	vec3 Right;

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Target, this->Up);
	}


	void Move(MoveData &movedata)
	{
		this->Position += movedata.Speed * this->Target;
		this->Position -= movedata.Speed * this->Target;
		this->Position -= glm::normalize(glm::cross(this->Target, Up)) * movedata.Speed;
		this->Position += glm::normalize(glm::cross(this->Target, Up)) * movedata.Speed;
	}

	void Rotate(RotateData &rotatedata)
	{
		glm::vec3 target;
		target.x = cos(glm::radians(rotatedata.Pitch)) * cos(glm::radians(rotatedata.Yaw));
		target.y = sin(glm::radians(rotatedata.Pitch));
		target.z = cos(glm::radians(rotatedata.Pitch)) * sin(glm::radians(rotatedata.Yaw));
		
		this->Target = glm::normalize(target);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Target, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = glm::normalize(glm::cross(this->Right, this->Target));
	}
};

