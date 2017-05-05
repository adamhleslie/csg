#pragma once

#include <vector>
#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>

enum Color
{
	GREEN, BLUE, RED, GREY, WHITE, DEFAULT
};

class Triangle
{
public:
	Triangle () {}
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, Color color = DEFAULT);

	void updatePlane();
	void classifyTriangle(Triangle other, std::vector<Triangle>& on, std::vector<Triangle>& front, std::vector<Triangle>& back) const;
	void addToRenderBuffer(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals, std::vector<glm::vec3>& colors) const;
	void addLinesToRenderBuffer(std::vector<glm::vec4>& vertices, std::vector<glm::uvec2>& lines) const;

	void flip();
	void applyMatrix(glm::mat4 matrix);
	void splitAndExtend(std::vector<Triangle>& meshTriangles, glm::vec3 extension, bool addColor = false) const;
	void splitAndExtend(std::vector<Triangle>& meshTriangles, float extension, bool addColor = false) const;
	void splitAndExtendNormalized(std::vector<Triangle>& meshTriangles, float extension, bool addColor = false) const;

	friend std::ostream& operator<< (std::ostream& os, const Triangle& obj)
	{
		os << glm::to_string(obj.mA) << "\n" << glm::to_string(obj.mB) << "\n" << glm::to_string(obj.mC) << std::endl;
		return os;
	}

private:
	glm::vec3 findPlaneIntersect(glm::vec3 origin, glm::vec3 direction) const;

	glm::vec3 mA;
	glm::vec3 mB;
	glm::vec3 mC;

	float mD;
	glm::vec3 mNormal;
	Color mColor;
};