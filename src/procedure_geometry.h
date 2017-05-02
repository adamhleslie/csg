#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>
#include "triangle.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
// FIXME: Add functions to generate the bone mesh.

glm::vec3 generateColor(Color color);

// Circular shapes
std::vector<Triangle> generateCylinder(int numPoints, float height, float radius, Color color = DEFAULT);
std::vector<Triangle> generateCone(int numPoints, float height, float radius, Color color = DEFAULT);
std::vector<Triangle> generateSphere(int numExtensions, float radius, Color color = DEFAULT);

// Prism shapes
std::vector<Triangle> generateDiamond(float length, float width, float height, Color color = DEFAULT);
std::vector<Triangle> generateRectangularPrism(float length, float width, float height, Color color = DEFAULT);
std::vector<Triangle> generateTriangularPrism(float length, float width, float height, Color color = DEFAULT);

void extendTriangles(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, glm::vec3 extension, bool addColor = false);
void extendTriangles(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, float extension, bool addColor = false);
void extendTrianglesNormalized(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, float extension, bool addColor = false);

#endif
