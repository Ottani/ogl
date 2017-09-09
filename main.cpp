#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using std::cout;
using std::cerr;

// TODO remove globals
int g_width = 640;
int g_height = 480;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int createShader();

int main(void)
{
	cout << "starting..." << std::endl;
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  	window = glfwCreateWindow(g_width, g_height, "Hello World", NULL, NULL);
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
	

	unsigned int shader = createShader();

	glViewport(0, 0, g_width, g_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f  // top left
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        
		glUseProgram(shader);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
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

unsigned int createVertexShader()
{
	std::ifstream f("vertex.glsl");
	std::stringstream strStream;
	strStream << f.rdbuf();
	std::string str = strStream.str();
	const char *cStr = str.c_str();
	
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cStr, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	return vertexShader;
}

unsigned int createFragmentShader()
{
	std::ifstream f("fragment.glsl");
	std::stringstream strStream;
	strStream << f.rdbuf();
	std::string str = strStream.str();
	const char *cStr = str.c_str();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cStr, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	return fragmentShader;
}

unsigned int createShader()
{
	unsigned int vertexShader = createVertexShader();
	unsigned int fragmentShader = createFragmentShader();

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
    char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
