#include <GL\glew.h>
#include <glfw3.h>
#include <string>

#pragma once

//callbackthingy
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
typedef void(*keycallback)(GLFWwindow*, int, int, int, int);
//static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
typedef void(*mmcallback)(GLFWwindow*, double, double);
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
typedef void(*mbcallback)(GLFWwindow*, int, int, int);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
typedef void(*msccallback)(GLFWwindow*, double, double);

class GameWindow
{
public:
	GameWindow();
	~GameWindow();

	bool initialize(const GLuint _contextmaj, const GLuint _contextmin, const GLuint _sizex, const GLuint _sizey, const std::string& _title);


private:
	GLFWwindow* m_window;
};