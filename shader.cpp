#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include "shader.hpp"

using std::cout;
using std::cerr;

const std::string readFile(const std::string &filename);

Shader::Shader() : shader(0), tmpShaders{0, 0}
{

}

Shader::~Shader()
{
	if (shader != 0 ) {
		glDeleteShader(shader);
	}
}

bool Shader::compile(const std::string& filename, ShaderType shaderType)
{
	cout << "loading shader " << filename << ", type: " << shaderType << '\n';
	if (tmpShaders[shaderType] != 0 || shader != 0) {
		cerr << "Error: Shader slot already used or already linked: " << shaderType << std::endl;
		return false;
	}

	const std::string contents = readFile(filename);
	const char * cStr = contents.c_str();
	
	tmpShaders[shaderType] = glCreateShader(shaderType==VERTEX?GL_VERTEX_SHADER:GL_FRAGMENT_SHADER);
	glShaderSource(tmpShaders[shaderType], 1, &cStr, NULL);
	glCompileShader(tmpShaders[shaderType]);

	int success;
	char infoLog[512];
	glGetShaderiv(tmpShaders[shaderType], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(tmpShaders[shaderType], 512, NULL, infoLog);
		cerr << "Error: shader compilation failed: " << infoLog << std::endl;
		glDeleteShader(tmpShaders[shaderType]);
		tmpShaders[shaderType] = 0;
		return false;
	}
	return true;
}

bool Shader::link()
{
	cout << "linking shader...\n";
	if (tmpShaders[VERTEX] == 0 || tmpShaders[FRAGMENT] == 0 || shader != 0 ) {
		cerr << "Error: Shaders not ready or already linked!" << std::endl;
		return false;
	}
	shader = glCreateProgram();
	glAttachShader(shader, tmpShaders[VERTEX]);
	glAttachShader(shader, tmpShaders[FRAGMENT]);
	glLinkProgram(shader);

	int success;
    char infoLog[512];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		cerr << "Error: link failed: " << infoLog << std::endl;
		return false;
	}
	glDeleteShader(tmpShaders[VERTEX]);
	glDeleteShader(tmpShaders[FRAGMENT]);
	return true;
}

const std::string readFile(const std::string &filename)
{
	std::ifstream f(filename);
	std::stringstream strStream;
	strStream << f.rdbuf();
	return strStream.str();
}
