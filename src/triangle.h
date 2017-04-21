#pragma once

#include <vector>
#include <glm/glm.hpp>

class Triangle
{
public:
	Triangle () {}
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

	void classifyTriangle(Triangle other, std::vector<Triangle>& on, std::vector<Triangle>& front, std::vector<Triangle>& back);

// private:
	glm::vec3 findPlaneIntersect(glm::vec3 origin, glm::vec3 direction);

	glm::vec3 mA;
	glm::vec3 mB;
	glm::vec3 mC;

	float mD;
	glm::vec3 mNormal;
};