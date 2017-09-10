#pragma once

#include <string>

class Shader
{
public:
	Shader();
	~Shader();
	Shader(const Shader& g) = delete;
	Shader(const Shader&& g) = delete;
	Shader& operator= (const Shader& g) = delete;
	Shader& operator= (const Shader&& g) = delete;

	enum class ShaderType : int
	{
		VERTEX = 0, FRAGMENT = 1
	};

	bool compile(const std::string& filename, ShaderType shaderType);
	unsigned int getShader() const { return shader; }

private:
	unsigned int shader;

};

