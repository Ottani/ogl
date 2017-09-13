#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cube.hpp"

Cube::Cube(glm::vec3 pos, float angle, float av, glm::vec3 rot)
	: pos(pos), angle(angle), av(av), rot(rot), model()
{

}

Cube::~Cube()
{

}

void Cube::update()
{
	glm::mat4 _model = glm::mat4();
	model = glm::translate(_model, pos);
	angle += av;
	model = glm::rotate(model, glm::radians(angle), rot);
}

//void Cube::draw()
//{
	//program->setMatrixValue("model", glm::value_ptr(model));
	//glDrawElements(GL_TRIANGLES, geometry->getQty(), GL_UNSIGNED_INT, 0);
//}
