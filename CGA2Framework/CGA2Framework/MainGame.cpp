#include "MainGame.h"

const GLfloat MainGame::vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

MainGame::MainGame(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver)
	: GameWindow(sizex, sizey, cvmaj, cvmin, title, uselatestglver)
{


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	

	glBindVertexArray(0); // Unbind VAO

	cam = new Camera();
	shader = new Shader("Sample.vs", "Sample.fs");
	shader->Use();
}


MainGame::~MainGame()
{
	delete cam;
	delete shader;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void MainGame::keycallback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void MainGame::mmcallback(double xpos, double ypos)
{
	//fprintf(stderr, "Mousepos: X: %.0f Y: %.0f\n", xpos, ypos);
}
void MainGame::mbcallback(int button, int action, int mods)
{

}

void MainGame::mscrcallback(double xoffset, double yoffset)
{

}

GLvoid MainGame::update(GLdouble time)
{
	MoveData md = { 0, 0, 0, 0, 0 };
	
	md.Speed = 0.01f;
	if (keys[GLFW_KEY_W])
		md.Forward = 2.0f;
	if (keys[GLFW_KEY_S])
		md.Backward = 2.0f;
	if (keys[GLFW_KEY_A])
		md.Left = 2.0f;
	if (keys[GLFW_KEY_D])
		md.Right = 2.0f;
	cam->Move(md);
}

GLvoid MainGame::render(GLdouble time)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view;
	glm::mat4 projection;
	view = cam->GetViewMatrix();
	//projection = glm::perspective(45.0f, (GLfloat)this->WIDTH() / (GLfloat)this->HEIGHT(), 0.1f, 100.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(45.0f, (GLfloat)this->WIDTH() / (GLfloat)this->HEIGHT(), 0.1f, 100.0f);
	shader->setUniform("view", view, false);
	shader->setUniform("projection", projection, false);

	glBindVertexArray(VAO);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));
	shader->setUniform("model", model, false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glfwSwapBuffers(this->m_window);
}
