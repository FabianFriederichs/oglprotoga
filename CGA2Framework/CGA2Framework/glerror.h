#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL\glew.h>

#define EXIT_ON_GL_ERROR 1
#define LOG_GL_ERRORS 1


#ifdef GLDEBUG
#define GLERR printglerror(__FILE__, __LINE__);
#else
#define GLERR
#endif

void printglerror(const char* file, int line);
//check for error, print it and return true/false if error occured
bool checkglerror_(const char* file, int line);
	
#define checkglerror() checkglerror_(__FILE__, __LINE__)