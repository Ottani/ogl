#include <iostream>
#include <string>
#include <cmath>
#include <cstdint>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

#include "window.hpp"
#include "cube.hpp"
#include "renderer.hpp"

using std::cout;
using std::cerr;

class MainApp
{
public:
	MainApp();
	~MainApp();
	MainApp(const MainApp& g) = delete;
	MainApp(const MainApp&& g) = delete;
	MainApp& operator= (const MainApp& g) = delete;
	MainApp& operator= (const MainApp&& g) = delete;

	bool init();
	void run();

private:
	void processInput();
	Window window;
	Renderer renderer;


	uint32_t lastFrame;
	uint32_t delta;
	bool running;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	
	float yaw;
	float pitch;
	float fov;
};

MainApp::MainApp() : window(1024, 768, "OPENGL"), cameraPos(0.0f, 0.0f,  3.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f,  0.0f), yaw(-90.0f), pitch(0.0f), fov(45.0f)
{

}

MainApp::~MainApp()
{
	SDL_Quit();
}

bool MainApp::init()
{
	cout << "starting..." << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	if (!window.init()) {
		return false;
	}

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cerr << "Error: " << glewGetErrorString(err) << '\n';
		return false;
	}

	return true;
}

void MainApp::run()
{
	cout << "initProgram\n";
	if (!renderer.initProgram("vertex04.glsl", "fragment04.glsl")) {
		cout << "exiting\n";
		return;
	}
	
	cout << "loadTextures\n";
	std::vector<std::string> filenames { "container.png", "awesomeface.png" };
	if (!renderer.loadTextures(filenames)) {
		cout << "exiting\n";
		return;
	}

	renderer.loadModel("cube_vert.txt", "cube_idx.txt");

	lastFrame = SDL_GetTicks();

	std::vector<Cube> cubes = {
		Cube(glm::vec3(0.0f,  0.0f,  0.0f),    5.0f,  1.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(2.0f,  5.0f, -15.0f),   5.0f,  2.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(-1.5f, -2.2f, -2.5f),   5.0f,  3.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(-3.8f, -2.0f, -12.3f),  5.0f,  4.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(2.4f, -0.4f, -3.5f),    5.0f,  5.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(-1.7f,  3.0f, -7.5f),   5.0f,  6.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(1.3f, -2.0f, -2.5f),    5.0f,  7.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(1.5f,  2.0f, -2.5f),    5.0f,  8.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(1.5f,  0.2f, -1.5f),    5.0f,  9.0f, glm::vec3(0.5f, 1.0f, 0.0f)),
		Cube(glm::vec3(-1.3f,  1.0f, -1.5f),   5.0f, 10.0f, glm::vec3(0.5f, 1.0f, 0.0f))
	};
	running = true;
	while (running) {
		uint32_t currFrame = SDL_GetTicks();
		delta = currFrame - lastFrame;
		lastFrame = currFrame;  
		processInput();

		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		renderer.render(view, projection, cubes);
		window.swap();
	}

}

void MainApp::processInput()
{
	SDL_Event event;
	while ((SDL_PollEvent(&event)) != 0) {
		if (event.type == SDL_QUIT) { 
			running = false;
			return;
		}
	}
	float cameraSpeed = 2.5f * (delta / 1000.0f);
	const uint8_t *state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE]) {
		running = false;
	}
	if (state[SDL_SCANCODE_W]) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (state[SDL_SCANCODE_S]) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (state[SDL_SCANCODE_A]) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (state[SDL_SCANCODE_D]) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	
	int xOffset, yOffset;
	const uint32_t mouseState = SDL_GetRelativeMouseState(&xOffset, &yOffset);

	float sensitivity = 0.15f;
	yaw   += (xOffset * sensitivity);
	pitch -= (yOffset * sensitivity);

	if(pitch > 89.0f) pitch = 89.0f;
	if(pitch < -89.0f) pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	//TODO missing
	/*if(fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if(fov <= 1.0f)
		fov = 1.0f;
	if(fov >= 45.0f)
		fov = 45.0f;*/
}

int main(void)
{
	MainApp app;
	if (!app.init()) {
		return -1;
	}
	app.run();

	return 0;
}

