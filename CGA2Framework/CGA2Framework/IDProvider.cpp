#include "IDProvider.h"


IDProvider::IDProvider() :
	m_lastid(0)
{
	
}


IDProvider::~IDProvider()
{

}

IDProvider::IDProvider(const IDProvider& _prov)
{

}

GLint IDProvider::createID()
{
	return m_lastid++;
}
