#pragma once

#include "bsp_tree.h"

class Object
{
public:
	Object () {}
	Object(const std::vector<Triangle>& meshTriangles);

	void render(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals, std::vector<glm::vec3>& colors, std::vector<glm::vec4>& line_vertices, std::vector<glm::uvec2>& lines) const;

// private:
	BspTree mTree;
	std::vector<Triangle> mTriangles;
};