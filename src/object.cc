#include "object.h"
#include <iostream>

Object::Object (const std::vector<Triangle>& meshTriangles) : 
	mTree(meshTriangles)
{
	mTree.buildTree(); 
	mTriangles = mTree.getTriangles();
}

void Object::render (std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals, std::vector<glm::vec3>& colors, std::vector<glm::vec4>& line_vertices, std::vector<glm::uvec2>& lines) const
{
	for (const Triangle& triangle : mTriangles)
	{
		triangle.addToRenderBuffer(vertices, faces, normals, colors);
		triangle.addLinesToRenderBuffer(line_vertices, lines);
	}
}

void Object::unify(Object& left, Object& right, Object& result)
{
	std::vector<Triangle> triangles;
	right.mTree.mergeTrees(left.mTriangles, nullptr, &triangles, true);
	left.mTree.mergeTrees(right.mTriangles, nullptr, &triangles, true);

	result = Object(triangles);
}

void Object::intersection(Object& left, Object& right, Object& result)
{
	std::vector<Triangle> triangles;
	right.mTree.mergeTrees(left.mTriangles, &triangles, nullptr, false);
	left.mTree.mergeTrees(right.mTriangles, &triangles, nullptr, false);

	result = Object(triangles);
}

void Object::difference(Object& left, Object& right, Object& result)
{
	std::vector<Triangle> leftOutside, rightInside;
	right.mTree.mergeTrees(left.mTriangles, nullptr, &leftOutside, true);
	left.mTree.mergeTrees(right.mTriangles, &rightInside, nullptr, false);

	for (Triangle& triangle : rightInside)
	{
		triangle.flip();
	}
	leftOutside.insert(leftOutside.end(), rightInside.begin(), rightInside.end());

	result = Object(leftOutside);
}