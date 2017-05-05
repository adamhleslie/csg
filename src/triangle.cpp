#include "triangle.h"
#include "config.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "procedure_geometry.h"

Triangle::Triangle (glm::vec3 a, glm::vec3 b, glm::vec3 c, Color color) :
	mA(a), mB(b), mC(c), mColor(color)
{
	updatePlane();
}

void Triangle::updatePlane ()
{
	mNormal = glm::cross(mB - mA, mC - mA);
	mD = -glm::dot(mNormal, mA);
}

void Triangle::classifyTriangle (Triangle other, std::vector<Triangle>& on, std::vector<Triangle>& front, std::vector<Triangle>& back) const
{
	// Calculate relative position of other triangle's points
	float a = glm::dot(mNormal, other.mA - mA);
	float b = glm::dot(mNormal, other.mB - mA);
	float c = glm::dot(mNormal, other.mC - mA);

	// Fix floating point error
	if (glm::abs(a) < kError)
	{
		a = 0;
	}
	if (glm::abs(b) < kError)
	{
		b = 0;
	}
	if (glm::abs(c) < kError)
	{
		c = 0;
	}

	// Determine relative position of entire triangle based on its points 
	if (a == 0 && b == 0 && c == 0)
	{
		// Directly on the plane
		on.push_back(other);
	}
	else if (a >= 0 && b >= 0 && c >= 0)
	{
		// In front of the plane
		front.push_back(other);
	}
	else if (a <= 0 && b <= 0 && c <= 0)
	{
		// In back of the plane
		back.push_back(other);
	}
	else
	{
		int signA = (a > 0) - (a < 0);
		int signB = (b > 0) - (b < 0);
		int signC = (c > 0) - (c < 0);

		// assert(a != 0 && b != 0 && c != 0);
		if (a == 0 || b == 0 || c == 0)
		{
			// Cut in half by plane, set so that c is the one on the plane
			if (a == 0)
			{
				glm::vec3 temp = other.mA;
				other.mA = other.mB;
				other.mB = other.mC;
				other.mC = temp;
				a = b;
			}
			else if (b == 0)
			{
				glm::vec3 temp = other.mB;
				other.mB = other.mA;
				other.mA = other.mC;
				other.mC = temp;
				a = c;
			}
			else if (c == 0)
			{
				assert(signA == -signB);
			}

			// Calculate the new vertex
			glm::vec3 D = findPlaneIntersect(other.mA, other.mB - other.mA);

			// Create two new triangles using the new vertex
			if (a > 0)
			{
				front.push_back(Triangle(D, other.mC, other.mA, other.mColor));
				back.push_back(Triangle(D, other.mB, other.mC, other.mColor));
			}
			else
			{
				back.push_back(Triangle(D, other.mC, other.mA, other.mColor));
				front.push_back(Triangle(D, other.mB, other.mC, other.mColor));
			}
		}
		else
		{
			// Spanning the plane, set so that c is the one on its own side
			if (signB == signC)
			{
				glm::vec3 temp = other.mA;
				other.mA = other.mB;
				other.mB = other.mC;
				other.mC = temp;
				c = a;
			}
			else if (signA == signC)
			{
				glm::vec3 temp = other.mB;
				other.mB = other.mA;
				other.mA = other.mC;
				other.mC = temp;
				c = b;
			}
			else
			{
				assert(signA == signB);
			}

			// Calculate the new vertices
			glm::vec3 A = findPlaneIntersect(other.mA, other.mC - other.mA);
			glm::vec3 B = findPlaneIntersect(other.mB, other.mC - other.mB);

			// Create the three new triangles using the new vertices
			if (c > 0)
			{
				back.push_back(Triangle(other.mA, other.mB, A, other.mColor));
				back.push_back(Triangle(other.mB, B, A, other.mColor));
				front.push_back(Triangle(A, B, other.mC, other.mColor));
			}
			else
			{
				front.push_back(Triangle(other.mA, other.mB, A, other.mColor));
				front.push_back(Triangle(other.mB, B, A, other.mColor));
				back.push_back(Triangle(A, B, other.mC, other.mColor));
			}
		}
	}
}

void Triangle::addToRenderBuffer (std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals, std::vector<glm::vec3>& colors) const
{
	assert(vertices.size() == normals.size() && vertices.size() == colors.size());
	unsigned size = vertices.size();

	vertices.push_back(glm::vec4(mA, 1));
	vertices.push_back(glm::vec4(mB, 1));
	vertices.push_back(glm::vec4(mC, 1));
	normals.insert(normals.end(), 3, glm::vec4(mNormal, 1));
	colors.insert(colors.end(), 3, generateColor(mColor));

	faces.push_back(glm::uvec3(size, size + 1, size + 2));
}

void Triangle::addLinesToRenderBuffer (std::vector<glm::vec4>& vertices, std::vector<glm::uvec2>& lines) const
{
	unsigned size = vertices.size();

	vertices.push_back(glm::vec4(mA, 1));
	vertices.push_back(glm::vec4(mB, 1));
	vertices.push_back(glm::vec4(mC, 1));

	lines.push_back(glm::uvec2(size, size + 1));
	lines.push_back(glm::uvec2(size, size + 2));
	lines.push_back(glm::uvec2(size + 1, size + 2));
}

void Triangle::flip ()
{
	glm::vec3 temp = mA;
	mA = mB;
	mB = temp;
	
	updatePlane();
}

void Triangle::applyMatrix (glm::mat4 matrix)
{
	mA = glm::vec3(matrix * glm::vec4(mA, 1));
	mB = glm::vec3(matrix * glm::vec4(mB, 1));
	mC = glm::vec3(matrix * glm::vec4(mC, 1));
	updatePlane();
}

void Triangle::splitAndExtend(std::vector<Triangle>& meshTriangles, glm::vec3 extension, bool addColor) const
{
	glm::vec3 p1 = mA;
	glm::vec3 p2 = mB;
	glm::vec3 p3 = mC;
	glm::vec3 p4 = p1 + ((p2 - p1) / 2.0f) + extension;
	glm::vec3 p5 = p2 + ((p3 - p2) / 2.0f) + extension;
	glm::vec3 p6 = p3 + ((p1 - p3) / 2.0f) + extension;

	meshTriangles.push_back(Triangle(p1, p4, p6, mColor));
	meshTriangles.push_back(Triangle(p4, p2, p5, mColor));
	meshTriangles.push_back(Triangle(p5, p6, p4, (Color) ((int) mColor + addColor)));
	meshTriangles.push_back(Triangle(p6, p5, p3, mColor));
}

void Triangle::splitAndExtend(std::vector<Triangle>& meshTriangles, float extension, bool addColor) const
{
	glm::vec3 p1 = mA;
	glm::vec3 p2 = mB;
	glm::vec3 p3 = mC;
	glm::vec3 p4 = (p1 + ((p2 - p1) / 2.0f)) * extension;
	glm::vec3 p5 = (p2 + ((p3 - p2) / 2.0f)) * extension;
	glm::vec3 p6 = (p3 + ((p1 - p3) / 2.0f)) * extension;

	meshTriangles.push_back(Triangle(p1, p4, p6, mColor));
	meshTriangles.push_back(Triangle(p4, p2, p5, mColor));
	meshTriangles.push_back(Triangle(p5, p6, p4, (Color) ((int) mColor + addColor)));
	meshTriangles.push_back(Triangle(p6, p5, p3, mColor));
}

void Triangle::splitAndExtendNormalized(std::vector<Triangle>& meshTriangles, float extension, bool addColor) const
{
	glm::vec3 p1 = mA;
	glm::vec3 p2 = mB;
	glm::vec3 p3 = mC;
	glm::vec3 p4 = glm::normalize(p1 + ((p2 - p1) / 2.0f)) * extension;
	glm::vec3 p5 = glm::normalize(p2 + ((p3 - p2) / 2.0f)) * extension;
	glm::vec3 p6 = glm::normalize(p3 + ((p1 - p3) / 2.0f)) * extension;

	meshTriangles.push_back(Triangle(p1, p4, p6, mColor));
	meshTriangles.push_back(Triangle(p4, p2, p5, mColor));
	meshTriangles.push_back(Triangle(p5, p6, p4, (Color) ((int) mColor + addColor)));
	meshTriangles.push_back(Triangle(p6, p5, p3, mColor));
}

glm::vec3 Triangle::findPlaneIntersect (glm::vec3 origin, glm::vec3 direction) const
{
	float t = -(glm::dot(mNormal, origin) + mD) / (glm::dot(mNormal, direction));
	return origin + (t * direction);
}