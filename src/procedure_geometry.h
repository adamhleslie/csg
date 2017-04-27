#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>
#include "triangle.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
// FIXME: Add functions to generate the bone mesh.

void generateCylinder(std::vector<Triangle>& mesh_triangles, int numPoints, float height, float radius);
void generateCone(std::vector<Triangle>& mesh_triangles, int numPoints, float height, float radius);
void generateRectangularPrism(std::vector<Triangle>& mesh_triangles, float length, float width, float height);
void generateTriangularPrism(std::vector<Triangle>& mesh_triangles, float length, float width, float height);

#endif
