#pragma once
#include "headers.h"
#include "glerror.h"
class GLFWHandler
{
public:
	GLFWHandler();
	~GLFWHandler();	

	static GLFWHandler* handlerInstance;
	virtual void setHandlerInstance() { handlerInstance = this; }

	virtual void keycallback(int key, int scancode, int action, int mods) = 0;
	virtual void mmcallback(double xpos, double ypos) = 0;
	virtual void mbcallback(int button, int action, int mods) = 0;
	virtual void mscrcallback(double xoffset, double yoffset) = 0;

	//dispatchers
	static void key_dispatch(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (handlerInstance)
			handlerInstance->keycallback(key, scancode, action, mods);
	}

	static void mm_dispatch(GLFWwindow* window, double xpos, double ypos)
	{
		if (handlerInstance)
			handlerInstance->mmcallback(xpos, ypos);
	}

	static void mb_dispatch(GLFWwindow* window, int button, int action, int mods)
	{
		if (handlerInstance)
			handlerInstance->mbcallback(button, action, mods);
	}

	static void mscr_dispatch(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (handlerInstance)
			handlerInstance->mscrcallback(xoffset, yoffset);
	}

	
};

