#pragma once

#include <vector>
#include <ostream>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Triangle
{
public:
	Triangle () {}
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

	void classifyTriangle(Triangle other, std::vector<Triangle>& on, std::vector<Triangle>& front, std::vector<Triangle>& back);
	void addToRenderBuffer(std::vector<glm::vec4> vertices, std::vector<glm::uvec3> faces);

	friend std::ostream& operator<< (std::ostream& os, const Triangle& obj)
	{
		os << glm::to_string(obj.mA) << "\n" << glm::to_string(obj.mB) << "\n" << glm::to_string(obj.mC) << std::endl;
		return os;
	}

private:
	glm::vec3 findPlaneIntersect(glm::vec3 origin, glm::vec3 direction);

	glm::vec3 mA;
	glm::vec3 mB;
	glm::vec3 mC;

	float mD;
	glm::vec3 mNormal;
};