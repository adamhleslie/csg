#include "triangle.h"
#include "config.h"

Triangle::Triangle (glm::vec3 a, glm::vec3 b, glm::vec3 c) :
	mA(a), mB(b), mC(c)
{
	mNormal = glm::cross(b - a, c - a);
}

RelativePosition Triangle::classifyTriangle (Triangle& other)
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
	else if (glm::abs(b) < kError)
	{
		b = 0;
	}
	else if (glm::abs(c) < kError)
	{
		c = 0;
	}

	// Determine relative position of entire triangle based on its points 
	if (a == 0 && b == 0 && c == 0)
	{
		return kOn;
	}
	else if (a >= 0 && b >= 0 && c >= 0)
	{
		return kFront;
	}
	else if (a <= 0 && b <= 0 && c <= 0)
	{
		return kBack;
	}
	else
	{
		return kSpanning;
	}
}