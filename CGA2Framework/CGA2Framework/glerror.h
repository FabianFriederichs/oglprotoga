#pragma once
#include "headers.h"

#define EXIT_ON_GL_ERROR 0

#ifdef GLDEBUG
#define GLERR printglerror();
#else
#define GLERR
#endif

void printglerror();
	