#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

using std::cout;
using std::cerr;

template<class T>
std::vector<T> loadFromFile(const std::string &filename);

Renderer::Renderer()
{
	stbi_set_flip_vertically_on_load(true);

}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

bool Renderer::initProgram(const std::string& vertexFilename, const std::string& fragmentFilename)
{
	return program.link(vertexFilename, fragmentFilename);
}

bool Renderer::loadTextures(const std::vector<std::string>& filenames)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, 512, 512, filenames.size());
	cout << "GlEnum: " << glGetError() << '\n';
	int width, height, nrChannels;
	int i = 0;
	bool ret = true;
	for (auto& filename : filenames) {
		if (!ret) break;
		cout << "Loading texture " << filename << '\n';
		std::string texFilename = "resources/" + filename;
		unsigned char *data = stbi_load(texFilename.c_str(), &width, &height, &nrChannels, 0);
		if (width != 512 || height != 512) {
			cerr << "Wrong format" << texFilename << std::endl;
			ret = false;
		}
		else if (data) {
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 512, 512, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
			// TODO glGenerateMipmap(GL_TEXTURE_2D);
			++i;
		}
		else {
			cerr << "Failed to load texture: " << texFilename << std::endl;
			ret = false;
		}
		stbi_image_free(data);
	}
	return ret;
}

void Renderer::loadModel(const std::string& verticesFilename, const std::string& indicesFilename)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	auto vertices = loadFromFile<float>(verticesFilename);
	cout << "loaded " << vertices.size() << " vertices\n";
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	auto indices = loadFromFile<unsigned int>(indicesFilename);
	cout << "loaded " << indices.size() << " indices\n";
	qty = indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); 

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::render(const glm::mat4& view, const glm::mat4& projection, std::vector<Cube>& cubes)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(program.getShader());
	program.setValue("textureSampler", 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glBindVertexArray(VAO);
	
	program.setMatrixValue("view", glm::value_ptr(view));
	program.setMatrixValue("projection", glm::value_ptr(projection));
	for (auto& cube : cubes) {
		cube.update();
		program.setMatrixValue("model", glm::value_ptr(cube.getModel()));
		glDrawElements(GL_TRIANGLES, qty, GL_UNSIGNED_INT, 0);
	}

}

template<class T>
std::vector<T> loadFromFile(const std::string &filename)
{
	std::ifstream input("resources/" + filename);
	std::vector<T> vec;
	T f;
	while(input >> f)
	{
		vec.push_back(f);
	}
	return vec;
}
