#include "object.h"
#include <iostream>

Object::Object (const std::vector<Triangle>& meshTriangles) : 
	mTree(meshTriangles)
{ 
	mTree.buildTree(); 
	mTriangles = mTree.getTriangles();
}

void Object::render(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals, std::vector<glm::vec3>& colors, std::vector<glm::vec4>& line_vertices, std::vector<glm::uvec2>& lines) const
{
	for (const Triangle& triangle : mTriangles)
	{
		triangle.addToRenderBuffer(vertices, faces, normals, colors);
		triangle.addLinesToRenderBuffer(line_vertices, lines);
		// std::cout << triangle << std::endl;
	}
}