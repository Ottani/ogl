#pragma once

#include <string>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height, const std::string& title);
	~Window();
	Window(const Window& g) = delete;
	Window(const Window&& g) = delete;
	Window& operator= (const Window& g) = delete;
	Window& operator= (const Window&& g) = delete;

	GLFWwindow* getWindow() const { return window; };
	static auto resizeCallback(GLFWwindow* window, int width, int height) -> void {
		Window *w = static_cast<Window*>(glfwGetWindowUserPointer(window));
		w->resize(width, height);
	}

private:
	void resize(int newWidth, int newHeight);
	GLFWwindow* window;
	int width;
	int height;
};
