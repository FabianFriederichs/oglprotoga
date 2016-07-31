#pragma once
#include "headers.h"
using namespace glm;

enum class MovementType :int
{
	FORWARD =1,
	BACKWARD=2,
	STRAFE_L =4, 
	STRAFE_R=8

};

inline MovementType operator~ (MovementType a) { return (MovementType)~(int)a; }
inline MovementType operator| (MovementType a, MovementType b) { return (MovementType)((int)a | (int)b); }
inline MovementType operator& (MovementType a, MovementType b) { return (MovementType)((int)a & (int)b); }
inline MovementType operator^ (MovementType a, MovementType b) { return (MovementType)((int)a ^ (int)b); }
inline MovementType& operator|= (MovementType& a, MovementType b) { return (MovementType&)((int&)a |= (int)b); }
inline MovementType& operator&= (MovementType& a, MovementType b) { return (MovementType&)((int&)a &= (int)b); }
inline MovementType& operator^= (MovementType& a, MovementType b) { return (MovementType&)((int&)a ^= (int)b); }
inline bool HasFlag(MovementType a, MovementType b){ if((a&b)==b) return true; else false; }
inline MovementType operator+ (MovementType a, MovementType b) { if(HasFlag(a,b)) return a; else return (MovementType)((int)a+(int)b); }
inline MovementType& operator+= (MovementType &a, MovementType b) { if(HasFlag(a,b)) return a; else return (MovementType&)((int&)a+=(int)b); }

struct MoveData
{
	vec3 mtype;
	GLfloat Multiplier;

	/*glm::vec4 asVec4() const
	{
		return vec4(Right,Up,Forward,Speed);
	}

	glm::vec3 asVec3() const
	{
		return vec3(Right,Up,Forward);
	}*/
};

class Moveable
{
public:
	virtual void Move(const MoveData &data) = 0;
};