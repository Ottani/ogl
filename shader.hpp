#pragma once

#include <string>

enum ShaderType 
{
	VERTEX = 0, FRAGMENT = 1
};

class Shader
{
public:
	Shader();
	~Shader();
	Shader(const Shader& g) = delete;
	Shader(const Shader&& g) = delete;
	Shader& operator= (const Shader& g) = delete;
	Shader& operator= (const Shader&& g) = delete;

	bool compile(const std::string& filename, ShaderType shaderType);
	bool link();

	unsigned int getShader() const { return shader; }

private:
	unsigned int tmpShaders[2];
	unsigned int shader;

};

