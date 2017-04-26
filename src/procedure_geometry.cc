#include "procedure_geometry.h"
#include "config.h"

void addCylinderTriangles(std::vector<Triangle>& mesh_triangles, int triangleIndex, glm::vec3 initial, glm::vec3 end, glm::vec3 offset0, glm::vec3 offset1);

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces)
{
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMin, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMin, 1.0f));
	floor_faces.push_back(glm::uvec3(0, 1, 2));
	floor_faces.push_back(glm::uvec3(2, 3, 0));
}

void generateCylinder(std::vector<Triangle>& mesh_triangles, glm::vec3 initial, glm::vec3 end)
{
	std::vector<glm::vec3> offsets(kNumCylinderPoints);

	// Calculate the normal and binormal for this cylinder
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
	glm::vec3 binormal = glm::cross(tangent, normal);

	double angle = 0;
	for (int i = 0; i < kNumCylinderPoints; ++i)
	{
		glm::vec3 offset = ((float) (kCylinderRadius * glm::cos(angle)) * normal) + ((float) (kCylinderRadius * glm::sin(angle)) * binormal);

		offsets[i] = offset;

		angle += glm::pi<float>() / (kNumCylinderPoints / 2);
	}

	mesh_triangles.resize(kNumCylinderPoints * 4);

	addCylinderTriangles(mesh_triangles, 0, initial, end, offsets[kNumCylinderPoints - 1], offsets[0]);
	for (int i = 1; i < kNumCylinderPoints; ++i)
	{
		int triangleIndex = i * 4;
		addCylinderTriangles(mesh_triangles, triangleIndex, initial, end, offsets[i - 1], offsets[i]);
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