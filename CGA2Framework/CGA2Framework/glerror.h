#pragma once
#include "headers.h"

#define EXIT_ON_GL_ERROR 0

#ifdef GLDEBUG
#define GLERR printglerror();
#else
#define GLERR
#endif

void printglerror();
//for printing a previous read error
void printglerror(GLenum err);

//check for error, print it and return true/false if error occured
bool checkglerror();
	