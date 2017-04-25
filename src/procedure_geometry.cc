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

// FIXME: create cylinders and lines for the bones
// Hints: Generate a lattice in [-0.5, 0, 0] x [0.5, 1, 0] We wrap this
// around in the vertex shader to produce a very smooth cylinder.  We only
// need to send a small number of points.  Controlling the grid size gives a
// nice wireframe.

// void generateCylinder(std::vector<Triangle>& mesh_triangles, glm::vec4 initial, glm::vec4 end, glm::vec4 normal, glm::vec4 binormal)
// {
// 	cylinder.vertices.resize(numCylinderPoints * 2);

// 	double angle = 0;
// 	for (int i = 0; i < numCylinderPoints; ++i)
// 	{
// 		glm::vec4 offset = ((float) (kCylinderRadius * glm::cos(angle)) * normal) + ((float) (kCylinderRadius * glm::sin(angle)) * binormal);

// 		cylinder.vertices[i] = initial + offset;
// 		cylinder.vertices[i + numCylinderPoints] = end + offset;

// 		angle += glm::pi<float>() / (numCylinderPoints / 2);
// 	}
// }
