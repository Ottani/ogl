#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "geometry.hpp"

using std::cout;
using std::cerr;

// TODO remove globals
int g_width = 640;
int g_height = 480;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void runLoop(GLFWwindow* window, const Shader& shader1, const Shader& shader2);

int main(void)
{
	cout << "starting..." << std::endl;
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  	window = glfwCreateWindow(g_width, g_height, "OPENGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (err = GLEW_OK)
	{
		cerr << "Error: " << glewGetErrorString(err) << '\n';
		return -2;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	Shader shader1, shader2;
	shader1.compile("vertex.glsl", VERTEX);
	shader1.compile("fragment.glsl", FRAGMENT);
	shader1.link();

	shader2.compile("vertex1.glsl", VERTEX);
	shader2.compile("fragment.glsl", FRAGMENT);
	shader2.link();

	glViewport(0, 0, g_width, g_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	runLoop(window, shader1, shader2);

	glfwTerminate();
	return 0;
}

void runLoop(GLFWwindow* window, const Shader& shader1, const Shader& shader2)
{
	Geometry g1, g2;
	g1.load("vertices01.txt", "indices01.txt");
	g2.load("vertices02.txt", "indices02.txt");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        
		glUseProgram(shader1.getShader());
		glBindVertexArray(g1.getVAO());
		glDrawElements(GL_TRIANGLES, g1.getQty(), GL_UNSIGNED_INT, 0);

		glUseProgram(shader2.getShader());
		glBindVertexArray(g2.getVAO());
		glDrawElements(GL_TRIANGLES, g2.getQty(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	g_width = width;
	g_height = height;
    glViewport(0, 0, width, height);
    cout << "W: " << width << " H: " << height << '\n';
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
