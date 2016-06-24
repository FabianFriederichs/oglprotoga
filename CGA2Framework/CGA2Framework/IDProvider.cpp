#include "IDProvider.h"
GLint IDProvider::m_lastid = 0;

GLint IDProvider::createID()
{
	return m_lastid++;
}
