#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include "geometry.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

using std::cout;
using std::cerr;

template<class T>
std::vector<T> loadFromFile(const std::string &filename);

Geometry::Geometry()
{

}

Geometry::~Geometry()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Geometry::load(const std::string& verticesFilename, const std::string& indicesFilename, VertStructType structType)
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

	switch (structType) {
		case VertStructType::VERTEX_ONLY:
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			break;
		case VertStructType::WITH_COLOR:
			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
			glEnableVertexAttribArray(1);
			break;
		case VertStructType::WITH_COLOR_TEXT:
			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
			glEnableVertexAttribArray(1);
			// texture attribute
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2); 
			break;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool Geometry::addTexture(const std::string& filename)
{
	cout << "Loading texture " << filename << '\n';
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	std::string texFilename = "resources/" + filename;
	unsigned char *data = stbi_load(texFilename.c_str(), &width, &height, &nrChannels, 0);
	bool ret = true;
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cerr << "Failed to load texture: " << texFilename << std::endl;
		ret = false;
	}
	stbi_image_free(data);
	return ret;
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
