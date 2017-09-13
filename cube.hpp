#pragma once

#include <glm/mat4x4.hpp>

class Cube
{
public:
	Cube(glm::vec3 pos, float angle, float av, glm::vec3 rot);
	~Cube();

	void update();
	glm::mat4 getModel() const { return model; };

private:
	glm::vec3 pos;
	float angle;
	float av;
	glm::vec3 rot;
	glm::mat4 model;
};
