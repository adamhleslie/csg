#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>
#include "triangle.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
// FIXME: Add functions to generate the bone mesh.

glm::vec3 generateColor(unsigned color);

// Circular shapes
void generateCylinder(std::vector<Triangle>& meshTriangles, int numPoints, float height, float radius, unsigned color = 0);
void generateCone(std::vector<Triangle>& meshTriangles, int numPoints, float height, float radius, unsigned color = 0);
void generateSphere(std::vector<Triangle>& meshTriangles, int numExtensions, float radius, unsigned color = 0);

// Prism shapes
void generateDiamond(std::vector<Triangle>& meshTriangles, float length, float width, float height, unsigned color = 0);
void generateRectangularPrism(std::vector<Triangle>& meshTriangles, float length, float width, float height, unsigned color = 0);
void generateTriangularPrism(std::vector<Triangle>& meshTriangles, float length, float width, float height, unsigned color = 0);

void extendTriangles(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, glm::vec3 extension, bool addColor = false);
void extendTriangles(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, float extension, bool addColor = false);
void extendTrianglesNormalized(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, float extension, bool addColor = false);

#endif
