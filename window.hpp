#pragma once

#include <string>
#include <SDL2/SDL.h>

class Window
{
public:
	Window(int width, int height, const std::string& title);
	~Window();
	Window(const Window& g) = delete;
	Window(const Window&& g) = delete;
	Window& operator= (const Window& g) = delete;
	Window& operator= (const Window&& g) = delete;

	bool init();
	void swap();

	SDL_Window* getWindow() const { return window; };
	//static auto resizeCallback(GLFWwindow* window, int width, int height) -> void {
	//	Window *w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	//	w->resize(width, height);
	//}

	int getWidth() const { return width; }
	int getHeight() const { return height; }

private:
	void resize(int newWidth, int newHeight);
	SDL_Window* window;
	SDL_GLContext glContext;
	int width;
	int height;
};
