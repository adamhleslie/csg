#pragma once

#include "bsp_tree.h"
#include <iostream>

class Object
{
public:
	Object () {}
	
	Object(const std::vector<Triangle>& meshTriangles);

	void render(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals, std::vector<glm::vec3>& colors, std::vector<glm::vec4>& line_vertices, std::vector<glm::uvec2>& lines) const;
	
	static void unify(Object& left, Object& right, Object& result);
	static void intersection(Object& left, Object& right, Object& result);
	static void difference(Object& left, Object& right, Object& result);

private:
	BspTree mTree;
	std::vector<Triangle> mTriangles;
};