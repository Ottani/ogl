#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "program.hpp"
#include "geometry.hpp"
#include "window.hpp"

using std::cout;
using std::cerr;

// TODO remove globals
void processInput(GLFWwindow *window);
void runLoop(const Window& window, const Program& program1, const Program& program2);

int main(void)
{
	cout << "starting..." << std::endl;

	Window window(640, 480, "OPENGL");
	if (!window.getWindow())
	{
		cerr << "Error creating window!\n";
		return -1;
	}

	GLenum err = glewInit();
	if (err = GLEW_OK)
	{
		cerr << "Error: " << glewGetErrorString(err) << '\n';
		return -2;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	Program program1, program2;
	program1.link("vertex.glsl", "fragment.glsl");
	program2.link("vertex1.glsl", "fragment.glsl");

	runLoop(window, program1, program2);

	return 0;
}

void runLoop(const Window& window, const Program& program1, const Program& program2)
{
	Geometry g1, g2;
	g1.load("vertices01.txt", "indices01.txt");
	g2.load("vertices02.txt", "indices02.txt");

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		processInput(window.getWindow());
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        
		glUseProgram(program1.getShader());
		glBindVertexArray(g1.getVAO());
		glDrawElements(GL_TRIANGLES, g1.getQty(), GL_UNSIGNED_INT, 0);

		glUseProgram(program2.getShader());
		glBindVertexArray(g2.getVAO());
		glDrawElements(GL_TRIANGLES, g2.getQty(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
