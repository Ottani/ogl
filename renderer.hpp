#pragma once

#include <string>
#include <vector>
#include <glm/mat4x4.hpp>

#include "program.hpp"
#include "cube.hpp"

class Renderer
{
public:
	Renderer();
	~Renderer();
	Renderer(const Renderer& g) = delete;
	Renderer(const Renderer&& g) = delete;
	Renderer& operator= (const Renderer& g) = delete;
	Renderer& operator= (const Renderer&& g) = delete;

	bool initProgram(const std::string& vertexFilename, const std::string& fragmentFilename);
	bool loadTextures(const std::vector<std::string>& filenames);
	void loadModel(const std::string& verticesFilename, const std::string& indicesFilename);
	void render(const glm::mat4& view, const glm::mat4& projection, std::vector<Cube>& cubes);

private:
	Program program;
	unsigned int texture;
	unsigned int VBO, VAO, EBO;
	size_t qty;



};