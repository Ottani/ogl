#pragma once

#include <string>

class Geometry
{
public:
	Geometry();
	~Geometry();
	Geometry(const Geometry& g) = delete;
	Geometry(const Geometry&& g) = delete;
	Geometry& operator= (const Geometry& g) = delete;
	Geometry& operator= (const Geometry&& g) = delete;

	void load(const std::string& verticesFilename, const std::string& indicesFilename, bool hasColor);
	unsigned int getVAO() { return VAO; };
	size_t getQty() { return qty; }

private:
	unsigned int VBO, VAO, EBO;
	size_t qty;
};
