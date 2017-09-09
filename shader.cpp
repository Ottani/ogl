#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include "shader.hpp"

using std::cout;
using std::cerr;

unsigned int createVertexShader(const std::string &filename);
unsigned int createFragmentShader(const std::string &filename);
const std::string readFile(const std::string &filename);

unsigned int createShader(const std::string& vertexFilename, const std::string& fragmentFilename)
{
	unsigned int vertexShader = createVertexShader(vertexFilename);
	unsigned int fragmentShader = createFragmentShader(fragmentFilename);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
    char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cerr << "Error: link failed: " << infoLog << std::endl;
		return 0;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

unsigned int createVertexShader(const std::string &filename)
{
	const std::string contents = readFile(filename);
	const char * cStr = contents.c_str();
	
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cStr, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cerr << "Error: shader compilation failed: " << infoLog << std::endl;
		return 0;
	}
	return vertexShader;
}

unsigned int createFragmentShader(const std::string &filename)
{
	const std::string contents = readFile(filename);
	const char * cStr = contents.c_str();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cStr, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cerr << "Error fragment compilation failed: " << infoLog << std::endl;
		return 0;
	}
	return fragmentShader;
}

const std::string readFile(const std::string &filename)
{
	std::ifstream f(filename);
	std::stringstream strStream;
	strStream << f.rdbuf();
	return strStream.str();
}
