#include <GL\glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "Shader.h"
#include "MainGame.h"
#include "GameObject.h"
#include "ResourceLoader.h"
#include "util.h"
using namespace std;


int main(void)
{
	//obj test stuff
	std::vector<GameObject> objs = ResourceLoader::loadOBJ("C:\\Users\\Fabian\\Desktop\\testob.obj");
	int dummy = 0;
	
	
	MainGame mg(800, 600, 4, 0, "TESTGAME", false);
	Shader testShader("Sample.vs", "Sample.fs");
	mg.setCursorVisble(false);
	mg.run();	
}