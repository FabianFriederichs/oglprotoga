#pragma once
#include "headers.h"
class IDProvider
{
private:
	IDProvider();
	IDProvider(const IDProvider&);
	~IDProvider();
	GLint m_lastid;
public:	
	static IDProvider& getInstance()
	{
		static IDProvider provider;
		return provider;
	}

	GLint createID();
};

