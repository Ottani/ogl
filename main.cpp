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
void runLoop(const Window& window, const Program& program1);

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// TODO add to geometry? use ref count?
	Program program; // program1, program2, program3;
	program.link("vertex04.glsl", "fragment04.glsl");

	runLoop(window, program);

	return 0;
}

void runLoop(const Window& window, const Program& program)
{
	Geometry g;
	g.load("vertices04.txt", "indices04.txt", Geometry::VertStructType::WITH_COLOR_TEXT);
	g.addTexture("container.png");
	g.addTexture("awesomeface.png");

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		processInput(window.getWindow());
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 model;
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);

		glUseProgram(program.getShader());
		program.setValue("texture1", 0);
		program.setValue("texture2", 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.getTexture(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g.getTexture(1));
		glBindVertexArray(g.getVAO());
		
		program.setMatrixValue("view", glm::value_ptr(view));
		program.setMatrixValue("projection", glm::value_ptr(projection));

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			//float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f * (i + 1.0f)), glm::vec3(0.5f, 1.0f, 0.0f));
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			program.setMatrixValue("model", glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, g.getQty(), GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
