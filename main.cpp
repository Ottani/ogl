#include <iostream>
#include <string>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program.hpp"
#include "geometry.hpp"
#include "window.hpp"

using std::cout;
using std::cerr;

void processInput(GLFWwindow *window);
void runLoop(const Window& window, const Program& program1, const Program& program2, const Program& program3);

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

	// TODO add to geometry? use ref count?
	Program program1, program2, program3;
	program1.link("vertex01.glsl", "fragment01.glsl");
	program2.link("vertex02.glsl", "fragment02.glsl");
	program3.link("vertex03.glsl", "fragment03.glsl");

	runLoop(window, program1, program2, program3);

	return 0;
}

void runLoop(const Window& window, const Program& program1, const Program& program2, const Program& program3)
{
	Geometry g1, g2, g3;
	g1.load("vertices01.txt", "indices01.txt", Geometry::VertStructType::VERTEX_ONLY);
	g2.load("vertices02.txt", "indices02.txt", Geometry::VertStructType::WITH_COLOR);
	g3.load("vertices03.txt", "indices03.txt", Geometry::VertStructType::WITH_COLOR_TEXT);
	g3.addTexture("container.png");

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		processInput(window.getWindow());
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program1.getShader());
		float greenValue = (std::sin(glfwGetTime()) / 2.0f) + 0.5f;
		program1.setVecValue("vertexColor", 0.0f, greenValue, 0.0f, 1.0f);
		program1.setValue("offset", greenValue / 2.0f);
		glBindVertexArray(g1.getVAO());
		glDrawElements(GL_TRIANGLES, g1.getQty(), GL_UNSIGNED_INT, 0);

		glUseProgram(program2.getShader());
		glBindVertexArray(g2.getVAO());
		glDrawElements(GL_TRIANGLES, g2.getQty(), GL_UNSIGNED_INT, 0);

		//glm::mat4 trans;
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		glm::mat4 trans;
		trans = glm::rotate(trans, (float)glfwGetTime()*4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.0f, 0.5f, 0.0f));
		
		glUseProgram(program3.getShader());
		glBindTexture(GL_TEXTURE_2D, g3.getTexture());
		glBindVertexArray(g3.getVAO());
		program3.setMatrixValue("transform", glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, g3.getQty(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
