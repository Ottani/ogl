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
