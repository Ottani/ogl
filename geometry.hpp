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

	enum class VertStructType
	{
		VERTEX_ONLY, WITH_COLOR, WITH_COLOR_TEXT
	};

	void load(const std::string& verticesFilename, const std::string& indicesFilename, VertStructType structType);
	bool addTexture(const std::string& filename);
	unsigned int getVAO() const { return VAO; };
	unsigned int getTexture() const { return texture; };
	size_t getQty() const { return qty; }

private:
	unsigned int VBO, VAO, EBO, texture;
	size_t qty;
};
