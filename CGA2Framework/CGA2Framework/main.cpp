#include <stdio.h>
#include <stdlib.h>
#include "MainGame.h"
using namespace std;

typedef enum {
	A,
	B,
	C
} TEST;

int main(void)
{
	MainGame mg(800, 600, 4, 3, "testgame", false, 4);
	mg.setCursorVisble(false);
	mg.run();

	
}