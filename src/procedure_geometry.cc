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

glm::vec3 generateColor(Color colorID)
{
	glm::vec3 color;
	switch (colorID)
	{
		case GREEN:
			color = glm::vec3(0, 1, 0);
			break;

		case BLUE:
			color = glm::vec3(0, 0, 1);
			break;

		case RED:
			color = glm::vec3(1, 0, 1);
			break;

		case GREY:
			color = glm::vec3(.5, .5, .5);
			break;

		case WHITE:
		default:
			color = glm::vec3(1, 1, 1);
	}
	return color;
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

void addCylinderTriangles(std::vector<Triangle>& meshTriangles, int triangleIndex, glm::vec3 initial, glm::vec3 end, glm::vec3 offset0, glm::vec3 offset1, Color color)
{
	glm::vec3 p1 = initial + offset0;
	glm::vec3 p2 = initial;
	glm::vec3 p3 = initial + offset1;
	glm::vec3 p4 = end + offset0;
	glm::vec3 p5 = end;
	glm::vec3 p6 = end + offset1;

	// Bottom Triangle
	meshTriangles[triangleIndex] = Triangle(p1, p2, p3, color);

	// Side Triangles
	meshTriangles[triangleIndex + 1] = Triangle(p1, p3, p4, color);
	meshTriangles[triangleIndex + 2] = Triangle(p3, p6, p4, color);

	// Top Triangle
	meshTriangles[triangleIndex + 3] = Triangle(p6, p5, p4, color);
}

void generateCylinder(std::vector<Triangle>& meshTriangles, int numPoints, float height, float radius, Color color)
{
	glm::vec3 initial = glm::vec3(0, -(height / 2), 0);
	glm::vec3 end = glm::vec3(0, height / 2, 0);

	std::vector<glm::vec3> offsets(numPoints);
	generateCircleOffsets(offsets, numPoints, radius, initial, end);

	meshTriangles.resize(numPoints * 4);

	// Special case first, when wrapping around from the 0 to the last index
	addCylinderTriangles(meshTriangles, 0, initial, end, offsets[numPoints - 1], offsets[0], color);
	for (int i = 1; i < numPoints; ++i)
	{
		int triangleIndex = i * 4;
		addCylinderTriangles(meshTriangles, triangleIndex, initial, end, offsets[i - 1], offsets[i], color);
	}
}

void addConeTriangles(std::vector<Triangle>& meshTriangles, int triangleIndex, glm::vec3 initial, glm::vec3 end, glm::vec3 offset0, glm::vec3 offset1, Color color)
{
	glm::vec3 p1 = initial + offset0;
	glm::vec3 p2 = initial;
	glm::vec3 p3 = initial + offset1;
	glm::vec3 p4 = end;

	// Bottom Triangle
	meshTriangles[triangleIndex] = Triangle(p1, p2, p3, color);

	// Top Triangle
	meshTriangles[triangleIndex + 1] = Triangle(p1, p3, p4, color);
}

void generateCone(std::vector<Triangle>& meshTriangles, int numPoints, float height, float radius, Color color)
{
	glm::vec3 initial = glm::vec3(0, -(height / 2), 0);
	glm::vec3 end = glm::vec3(0, height / 2, 0);

	std::vector<glm::vec3> offsets(numPoints);
	generateCircleOffsets(offsets, numPoints, radius, initial, end);

	meshTriangles.resize(numPoints * 2);

	// Special case first, when wrapping around from the 0 to the last index
	addConeTriangles(meshTriangles, 0, initial, end, offsets[numPoints - 1], offsets[0], color);
	for (int i = 1; i < numPoints; ++i)
	{
		int triangleIndex = i * 2;
		addConeTriangles(meshTriangles, triangleIndex, initial, end, offsets[i - 1], offsets[i], color);
	}
}

void generateSphere(std::vector<Triangle>& meshTriangles, int numExtensions, float radius, Color color)
{
	std::vector<Triangle> temp;
	float circumference = 2 * radius;
	generateDiamond(meshTriangles, circumference, circumference, circumference, color);
	for (int i = 0; i < numExtensions; ++i)
	{
		extendTrianglesNormalized(meshTriangles, temp, radius);
		temp.swap(meshTriangles);
	}
}

void generateDiamond(std::vector<Triangle>& meshTriangles, float length, float width, float height, Color color)
{
	width /= 2;
	height /= 2;
	length /= 2;

	glm::vec3 p1(-width, 0, 0);
	glm::vec3 p2(width, 0, 0);
	glm::vec3 p3(0, -height, 0);
	glm::vec3 p4(0, height, 0);
	glm::vec3 p5(0, 0, -length);
	glm::vec3 p6(0, 0, length);

	// Top triangles
	meshTriangles.push_back(Triangle(p4, p5, p1, color));
	meshTriangles.push_back(Triangle(p4, p1, p6, color));
	meshTriangles.push_back(Triangle(p4, p6, p2, color));
	meshTriangles.push_back(Triangle(p4, p2, p5, color));

	// Bottom triangles
	meshTriangles.push_back(Triangle(p3, p1, p5, color));
	meshTriangles.push_back(Triangle(p3, p6, p1, color));
	meshTriangles.push_back(Triangle(p3, p2, p6, color));
	meshTriangles.push_back(Triangle(p3, p5, p2, color));
}

void generateRectangularPrism(std::vector<Triangle>& meshTriangles, float length, float width, float height, Color color)
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
	meshTriangles.push_back(Triangle(p1, p2, p4, color, "Top"));
	meshTriangles.push_back(Triangle(p2, p3, p4, color, "Top"));

	// Bottom triangles
	meshTriangles.push_back(Triangle(p5, p8, p6, color, "Bottom"));
	meshTriangles.push_back(Triangle(p8, p7, p6, color, "Bottom"));

	// Front triangles
	meshTriangles.push_back(Triangle(p1, p5, p6, color, "Front"));
	meshTriangles.push_back(Triangle(p6, p2, p1, color, "Front"));

	// Back triangles
	meshTriangles.push_back(Triangle(p7, p4, p3, color, "Back"));
	meshTriangles.push_back(Triangle(p7, p8, p4, color, "Back"));

	// Right triangles
	meshTriangles.push_back(Triangle(p6, p3, p2, color, "Right"));
	meshTriangles.push_back(Triangle(p6, p7, p3, color, "Right"));

	// Left triangles
	meshTriangles.push_back(Triangle(p5, p4, p8, color, "Left"));
	meshTriangles.push_back(Triangle(p5, p1, p4, color, "Left"));
}

void generateTriangularPrism(std::vector<Triangle>& meshTriangles, float length, float width, float height, Color color)
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
	meshTriangles.push_back(Triangle(p1, p2, p3, color));

	// Bottom triangle
	meshTriangles.push_back(Triangle(p6, p5, p4, color));

	// Front triangles
	meshTriangles.push_back(Triangle(p1, p4, p5, color));
	meshTriangles.push_back(Triangle(p1, p5, p2, color));

	// Right triangles
	meshTriangles.push_back(Triangle(p2, p5, p3, color));
	meshTriangles.push_back(Triangle(p5, p6, p3, color));

	// Left triangles
	meshTriangles.push_back(Triangle(p1, p3, p4, color));
	meshTriangles.push_back(Triangle(p4, p3, p6, color));
}

void extendTriangles(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, glm::vec3 extension, bool addColor)
{
	newMeshTriangles.clear();
	newMeshTriangles.reserve(meshTriangles.size() * 4);
	for (Triangle triangle : meshTriangles)
	{
		// glm::vec3 extension = mNormal * extensionDistance;
		triangle.splitAndExtend(newMeshTriangles, extension, addColor);
	}
}

void extendTriangles(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, float extension, bool addColor)
{
	newMeshTriangles.clear();
	newMeshTriangles.reserve(meshTriangles.size() * 4);
	for (Triangle triangle : meshTriangles)
	{
		// glm::vec3 extension = mNormal * extensionDistance;
		triangle.splitAndExtend(newMeshTriangles, extension, addColor);
	}
}

void extendTrianglesNormalized(const std::vector<Triangle>& meshTriangles, std::vector<Triangle>& newMeshTriangles, float extension, bool addColor)
{
	newMeshTriangles.clear();
	newMeshTriangles.reserve(meshTriangles.size() * 4);
	for (Triangle triangle : meshTriangles)
	{
		triangle.splitAndExtendNormalized(newMeshTriangles, extension, addColor);
	}
}