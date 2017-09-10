#include <iostream>

#include "window.hpp"

using std::cout;
using std::cerr;

Window::Window(int width, int height, const std::string& title)
		: window(nullptr), width(width), height(height)
{
	if (!glfwInit()) return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window)
	{
		glfwMakeContextCurrent(window);
	}

	glViewport(0, 0, width, height);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
}

Window::~Window()
{
	glfwTerminate();
}

void Window::resize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
    glViewport(0, 0, width, height);
    cout << "W: " << width << " H: " << height << '\n';
}
