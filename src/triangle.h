#pragma once

#include <glm/glm.hpp>

enum RelativePosition
{
	kOn, kBack, kFront, kSpanning
};

class Triangle
{
public:
	Triangle () {}
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

	RelativePosition classifyTriangle(Triangle& other);

private:
	glm::vec3 mA;
	glm::vec3 mB;
	glm::vec3 mC;

	glm::vec3 mNormal;
};