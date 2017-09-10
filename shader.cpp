#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include "shader.hpp"

using std::cout;
using std::cerr;

const std::string readFile(const std::string &filename);

Shader::Shader() : shader(0)
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
	cout << "loading shader " << filename << ", type: " << static_cast<int>(shaderType) << '\n';
	const std::string contents = readFile(filename);
	const char * cStr = contents.c_str();
	
	shader = glCreateShader(shaderType==ShaderType::VERTEX?GL_VERTEX_SHADER:GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &cStr, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cerr << "Error: shader compilation failed: " << infoLog << std::endl;
		return false;
	}
	return true;
}

const std::string readFile(const std::string &filename)
{
	std::ifstream f("resources/" + filename);
	std::stringstream strStream;
	strStream << f.rdbuf();
	return strStream.str();
}
