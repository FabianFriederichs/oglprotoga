#include <GL\glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "MainGame.h"
using namespace std;


int main(void)
{
	MainGame mg(800, 600, 4, 0, "TESTGAME", false);
	mg.setCursorVisble(false);
	mg.run();
	//GLFWwindow* window;

	//if (!glfwInit())
	//{
	//	fprintf(stderr, "Failed to initialize GLFW\n");
	//	getchar();
	//	return -1;
	//}

	//glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//// Open a window and create its OpenGL context
	//window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
	//if (window == NULL){
	//	fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
	//	getchar();
	//	glfwTerminate();
	//	return -1;
	//}
	//glfwMakeContextCurrent(window);

	//// Initialize GLEW
	//if (glewInit() != GLEW_OK) {
	//	fprintf(stderr, "Failed to initialize GLEW\n");
	//	getchar();
	//	glfwTerminate();
	//	return -1;
	//}

	//// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	///* Loop until the user closes the window */
	//while (!glfwWindowShouldClose(window))
	//{
	//	/* Render here */

	//	/* Swap front and back buffers */
	//	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	glfwSwapBuffers(window);

	//	/* Poll for and process events */
	//	glfwPollEvents();
	//}

	//glfwTerminate();
	//return 0;
}