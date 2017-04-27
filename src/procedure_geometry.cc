#include "procedure_geometry.h"
#include "config.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces)
{
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMin, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMin, 1.0f));
	floor_faces.push_back(glm::uvec3(0, 1, 2));
	floor_faces.push_back(glm::uvec3(2, 3, 0));
}

void generateCircleOffsets(std::vector<glm::vec3>& offsets, int numPoints, float radius, glm::vec3 initial, glm::vec3 end)
{
	// Calculate the normal and binormal for this circle
	glm::vec3 tangent = glm::normalize(end - initial);
	glm::vec3 normal = tangent;
	int min;
	double min_val = DBL_MAX;
	for (int i = 0; i < 3; ++i)
	{
		if (glm::abs(normal[i]) < min_val)
		{
			min_val = normal[i];
			min = i;
		}
	}

	normal = glm::vec3(0.0, 0.0, 0.0);
	normal[min] = 1.0;
	normal = glm::normalize(glm::cross(tangent, normal));

	glm::vec3 binormal = glm::cross(tangent, normal) * radius;
	normal *= radius;

	offsets.resize(numPoints);
	double angle = 0;
	for (int i = 0; i < numPoints; ++i)
	{
		glm::vec3 offset = ((float) (glm::cos(angle)) * normal) + ((float) (glm::sin(angle)) * binormal);

		offsets[i] = offset;

		angle += glm::pi<float>() / (numPoints / 2);
	}
}

void addCylinderTriangles(std::vector<Triangle>& mesh_triangles, int triangleIndex, glm::vec3 initial, glm::vec3 end, glm::vec3 offset0, glm::vec3 offset1)
{
	glm::vec3 p1 = initial + offset0;
	glm::vec3 p2 = initial;
	glm::vec3 p3 = initial + offset1;
	glm::vec3 p4 = end + offset0;
	glm::vec3 p5 = end;
	glm::vec3 p6 = end + offset1;

	// Bottom Triangle
	mesh_triangles[triangleIndex] = Triangle(p1, p2, p3);

	// Side Triangles
	mesh_triangles[triangleIndex + 1] = Triangle(p1, p3, p4);
	mesh_triangles[triangleIndex + 2] = Triangle(p3, p6, p4);

	// Top Triangle
	mesh_triangles[triangleIndex + 3] = Triangle(p6, p5, p4);
}

void generateCylinder(std::vector<Triangle>& mesh_triangles, int numPoints, float height, float radius)
{
	glm::vec3 initial = glm::vec3(0, -(height / 2), 0);
	glm::vec3 end = glm::vec3(0, height / 2, 0);

	std::vector<glm::vec3> offsets(numPoints);
	generateCircleOffsets(offsets, numPoints, radius, initial, end);

	mesh_triangles.resize(numPoints * 4);

	// Special case first, when wrapping around from the 0 to the last index
	addCylinderTriangles(mesh_triangles, 0, initial, end, offsets[numPoints - 1], offsets[0]);
	for (int i = 1; i < numPoints; ++i)
	{
		int triangleIndex = i * 4;
		addCylinderTriangles(mesh_triangles, triangleIndex, initial, end, offsets[i - 1], offsets[i]);
	}
}

void addConeTriangles(std::vector<Triangle>& mesh_triangles, int triangleIndex, glm::vec3 initial, glm::vec3 end, glm::vec3 offset0, glm::vec3 offset1)
{
	glm::vec3 p1 = initial + offset0;
	glm::vec3 p2 = initial;
	glm::vec3 p3 = initial + offset1;
	glm::vec3 p4 = end;

	// Bottom Triangle
	mesh_triangles[triangleIndex] = Triangle(p1, p2, p3);

	// Top Triangle
	mesh_triangles[triangleIndex + 1] = Triangle(p1, p3, p4);
}

void generateCone(std::vector<Triangle>& mesh_triangles, int numPoints, float height, float radius)
{
	glm::vec3 initial = glm::vec3(0, -(height / 2), 0);
	glm::vec3 end = glm::vec3(0, height / 2, 0);

	std::vector<glm::vec3> offsets(numPoints);
	generateCircleOffsets(offsets, numPoints, radius, initial, end);

	mesh_triangles.resize(numPoints * 2);

	// Special case first, when wrapping around from the 0 to the last index
	addConeTriangles(mesh_triangles, 0, initial, end, offsets[numPoints - 1], offsets[0]);
	for (int i = 1; i < numPoints; ++i)
	{
		int triangleIndex = i * 2;
		addConeTriangles(mesh_triangles, triangleIndex, initial, end, offsets[i - 1], offsets[i]);
	}
}

void generateRectangularPrism(std::vector<Triangle>& mesh_triangles, float length, float width, float height)
{
	width /= 2;
	height /= 2;
	length /= 2;

	// Top points
	glm::vec3 p1(-width, height, length);
	glm::vec3 p2(width, height, length);
	glm::vec3 p3(width, height, -length);
	glm::vec3 p4(-width, height, -length);

	// Bottom points
	glm::vec3 p5(-width, -height, length);
	glm::vec3 p6(width, -height, length);
	glm::vec3 p7(width, -height, -length);
	glm::vec3 p8(-width, -height, -length);

	// Top triangles
	mesh_triangles.push_back(Triangle(p1, p2, p4));
	mesh_triangles.push_back(Triangle(p2, p3, p4));

	// Bottom triangles
	mesh_triangles.push_back(Triangle(p5, p8, p6));
	mesh_triangles.push_back(Triangle(p8, p7, p6));

	// Front triangles
	mesh_triangles.push_back(Triangle(p1, p5, p6));
	mesh_triangles.push_back(Triangle(p6, p2, p1));

	// Back triangles
	mesh_triangles.push_back(Triangle(p7, p4, p3));
	mesh_triangles.push_back(Triangle(p7, p8, p4));

	// Right triangles
	mesh_triangles.push_back(Triangle(p6, p3, p2));
	mesh_triangles.push_back(Triangle(p6, p7, p3));

	// Left triangles
	mesh_triangles.push_back(Triangle(p5, p4, p8));
	mesh_triangles.push_back(Triangle(p5, p1, p4));
}

void generateTriangularPrism(std::vector<Triangle>& mesh_triangles, float length, float width, float height)
{
	width /= 2;
	height /= 2;
	length /= 2;

	// Top points
	glm::vec3 p1(-width, height, length);
	glm::vec3 p2(width, height, length);
	glm::vec3 p3(0, height, -length);

	// Bottom points
	glm::vec3 p4(-width, -height, length);
	glm::vec3 p5(width, -height, length);
	glm::vec3 p6(0, -height, -length);

	// Top triangle
	mesh_triangles.push_back(Triangle(p1, p2, p3));

	// Bottom triangle
	mesh_triangles.push_back(Triangle(p6, p5, p4));

	// Front triangles
	mesh_triangles.push_back(Triangle(p1, p4, p5));
	mesh_triangles.push_back(Triangle(p1, p5, p2));

	// Right triangles
	mesh_triangles.push_back(Triangle(p2, p5, p3));
	mesh_triangles.push_back(Triangle(p5, p6, p3));

	// Left triangles
	mesh_triangles.push_back(Triangle(p1, p3, p4));
	mesh_triangles.push_back(Triangle(p4, p3, p6));
}