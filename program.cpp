#include <iostream>
#include <string>

#include <GL/glew.h>

#include "shader.hpp"
#include "program.hpp"

using std::cout;
using std::cerr;

Program::Program() : shader(0)
{

}

Program::~Program()
{
	if (shader != 0 ) {
		glDeleteShader(shader);
	}
}

bool Program::link(const std::string& vertexFilename, const std::string& fragmentFilename)
{
	cout << "linking shader...\n";
	if (shader != 0 ) {
		cerr << "Error: Shader already linked!" << std::endl;
		return false;
	}

	shader = glCreateProgram();

	Shader vShader;
	if(!vShader.compile(vertexFilename, Shader::ShaderType::VERTEX)) {
		return false;
	}

	Shader fShader;
	if (!fShader.compile(fragmentFilename, Shader::ShaderType::FRAGMENT)) {
		return false;
	}

	glAttachShader(shader, vShader.getShader());
	glAttachShader(shader, fShader.getShader());
	cout << "linking...\n";
	glLinkProgram(shader);

	int success;
    char infoLog[512];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		cerr << "Error: link failed: " << infoLog << std::endl;
		return false;
	}
	return true;
}

template<>
void Program::setValue<bool>(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shader, name.c_str()), (int)value); 
}

template<>
void Program::setValue<int>(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shader, name.c_str()), value); 
}

template<>
void Program::setValue<float>(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shader, name.c_str()), value);
}

void Program::setVecValue(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(shader, name.c_str()), v1, v2, v3, v4);
}

void Program::setMatrixValue(const std::string& name, const float *value) const
{
	glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, value);
}
