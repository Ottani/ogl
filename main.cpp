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

#include "program.hpp"
#include "geometry.hpp"
#include "window.hpp"

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
	void doLoop();
	void processInput();
	Window window;
	Program program;
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

MainApp::MainApp() : window(640, 480, "OPENGL"), cameraPos(0.0f, 0.0f,  3.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f,  0.0f), yaw(-90.0f), pitch(0.0f), fov(45.0f)
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
	// TODO add to geometry? use ref count?
	program.link("vertex04.glsl", "fragment04.glsl");
	lastFrame = SDL_GetTicks();
	doLoop();
}

void MainApp::doLoop()
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
	running = true;
	while (running) {
		uint32_t currFrame = SDL_GetTicks();
		delta = currFrame - lastFrame;
		lastFrame = currFrame;  
		processInput();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 model;
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view;
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 projection;
		//projection = glm::perspective(glm::radians(45.0f), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		projection = glm::perspective(glm::radians(fov), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);

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
			float angle = 20.0f * i;
			//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f * (i + 1.0f)), glm::vec3(0.5f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			program.setMatrixValue("model", glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, g.getQty(), GL_UNSIGNED_INT, 0);
		}

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
	pitch += (yOffset * sensitivity);

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
