#include <GL\glew.h>
#include <glfw3.h>

#pragma once
class GameWindow
{
public:
	GameWindow();
	~GameWindow();

	bool initialize();

private:
	GLFWwindow* m_window;
};

