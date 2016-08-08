#include "glerror.h"

void printglerror()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "An OpenGL error has occured:";
		std::string message;
		switch (err)
		{
		case GL_INVALID_ENUM:
			message = "invalid enum";
			break;
		case GL_INVALID_VALUE:
			message = "invalid value";
			break;
		case GL_INVALID_OPERATION:
			message = "invalid operation";
			break;
		case GL_STACK_OVERFLOW:
			message = "stack overflow";
			break;
		case GL_STACK_UNDERFLOW:
			message = "stack underflow";
			break;
		case GL_OUT_OF_MEMORY:
			message = "out of memory";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			message = "invalid framebuffer operation";
			break;
		case GL_CONTEXT_LOST:
			message = "context lost";
			break;
		default:
			message = "unknown gl error";
		}
		std::cout << "\n" << message << std::endl;

		std::fstream fs;
		fs.open("glerrorlog.txt", std::ios_base::out | std::ios_base::app);
		if (fs.is_open())
		{
			fs << "OpenGL error: " << message << std::endl;
			fs.close();
		}

		if (EXIT_ON_GL_ERROR)
			exit(EXIT_FAILURE);
	}
}