#include <GL\glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "Shader.h"
#include "MainGame.h"
#include "GameObject.h"
#include "OBJLoader.h"
#include "util.h"
#include "dds.h"
using namespace std;


int main(void)
{
	//Texture dds = loadDDSTex("E:\\Benutzer\\Fabian\\Desktop\\TextureStuff\\Testtextures\\testdds.DDS");
	MainGame mg(800, 600, 4, 0, "testgame", false);
	mg.setCursorVisble(true);
	mg.run();

	
}