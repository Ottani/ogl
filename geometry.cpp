#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include "geometry.hpp"

using std::cout;

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

void Geometry::load(const std::string& verticesFilename, const std::string& indicesFilename, bool hasColor)
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

	if (hasColor) {
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
		glEnableVertexAttribArray(1);
	} else {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
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
