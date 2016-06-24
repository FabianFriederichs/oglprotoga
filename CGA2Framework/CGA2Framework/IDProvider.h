#pragma once
#include "headers.h"
class IDProvider
{
private:
	static GLint m_lastid;
public:
	static GLint createID();
};

