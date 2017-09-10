#pragma once

class Program
{
public:
	Program();
	~Program();
	Program(const Program& g) = delete;
	Program(const Program&& g) = delete;
	Program& operator= (const Program& g) = delete;
	Program& operator= (const Program&& g) = delete;

	bool link(const std::string& vertexFilename, const std::string& fragmentFilename);
	unsigned int getShader() const { return shader; }

private:
	unsigned int shader;

};