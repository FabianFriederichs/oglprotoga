#include <stdio.h>
#include <stdlib.h>
#include "MainGame.h"
using namespace std;


int main(void)
{
	//Texture dds = loadDDSTex("E:\\Benutzer\\Fabian\\Desktop\\TextureStuff\\Testtextures\\testdds.DDS");
	MainGame mg(800, 600, 4, 3, "testgame", false, 4);
	mg.setCursorVisble(false);
	mg.run();

	
}