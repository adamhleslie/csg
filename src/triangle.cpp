#include "triangle.h"
#include "config.h"

Triangle::Triangle (glm::vec3 a, glm::vec3 b, glm::vec3 c) :
	mA(a), mB(b), mC(c)
{
	mNormal = glm::cross(b - a, c - a);
	mD = -glm::dot(mNormal, a);
}

void Triangle::classifyTriangle (Triangle other, std::vector<Triangle>& on, std::vector<Triangle>& front, std::vector<Triangle>& back)
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
		// Spanning the plane, spanning case: 2 triangle split
		assert(a != 0 && b != 0 && c != 0);

		int signA = (a > 0) - (a < 0);
		int signB = (b > 0) - (b < 0);
		int signC = (c > 0) - (c < 0);

		// Spanning the plane, set so that c is the one on its own side
		if (signB == signC)
		{
			glm::vec3 temp = other.mA;
			other.mA = other.mC;
			other.mC = other.mB;
			other.mB = temp;
		}
		else if (signA == signC)
		{
			glm::vec3 temp = other.mB;
			other.mB = other.mA;
			other.mA = other.mC;
			other.mC = temp;
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
			back.push_back(Triangle(other.mA, other.mB, A));
			back.push_back(Triangle(other.mB, B, A));
			front.push_back(Triangle(A, B, other.mC));
		}
		else
		{
			front.push_back(Triangle(other.mA, other.mB, A));
			front.push_back(Triangle(other.mB, B, A));
			back.push_back(Triangle(A, B, other.mC));
		}
	}
}

glm::vec3 Triangle::findPlaneIntersect (glm::vec3 origin, glm::vec3 direction)
{
	float t = -(glm::dot(mNormal, origin) + mD) / (glm::dot(mNormal, direction));
	return origin + (t * direction);
}