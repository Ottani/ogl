#include <iostream>
#include <GL/glew.h>

#include "window.hpp"

using std::cout;
using std::cerr;

Window::Window(int width, int height, const std::string& title)
		: window(nullptr), width(width), height(height)
{
	
}

bool Window::init()
{
	window = SDL_CreateWindow("OPENGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		cerr << "Could not create window: " << SDL_GetError() << '\n';
        return false;
	}

	glContext = SDL_GL_CreateContext(window);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	return true;
}

Window::~Window()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
}

void Window::swap()
{
	SDL_GL_SwapWindow(window);
}

void Window::resize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
    glViewport(0, 0, width, height);
    cout << "W: " << width << " H: " << height << '\n';
}
