#include "language_parsing.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stack>
#include "triangle.h"
#include "procedure_geometry.h"

std::vector<Triangle> readInPrimitive (std::string primitiveText)
{
	// if square bracket then the next thing is the type of primitive
	// then its position
	// then its rotation
	// then its necessary info

	char primitiveChar = primitiveText[0];
	primitiveText = primitiveText.substr(2); //skip the letter and the first comma
	std::vector<float> values;

	std::string delimiter = ",";

	size_t pos = 0;
	std::string token;
	while ((pos = primitiveText.find(delimiter)) != std::string::npos) {
		token = primitiveText.substr(0, pos);

		float value = strtof(token.c_str(), nullptr);
		values.push_back(value);

		primitiveText.erase(0, pos + delimiter.length());
	}

	float value = strtof(primitiveText.c_str(), nullptr);
	values.push_back(value);

	glm::vec3 translation(values[0], values[1], values[2]);
	glm::vec3 rotation(values[3], values[4], values[5]);
	size_t arg = 6;

	std::vector<Triangle> triangles;
	switch (primitiveChar)
	{
		case 'C':
			triangles = generateCylinder(values[arg], values[arg + 1], values[arg + 2], (Color) values[arg + 3]);
			break;

		case 'K':
			triangles = generateCone(values[arg], values[arg + 1], values[arg + 2], (Color) values[arg + 3]);
			break;

		case 'S':
			triangles = generateSphere(values[arg], values[arg + 1], (Color) values[arg + 2]);
			break;

		case 'D':
			triangles = generateDiamond(values[arg], values[arg + 1], values[arg + 2], (Color) values[arg + 3]);
			break;

		case 'R':
			triangles = generateRectangularPrism(values[arg], values[arg + 1], values[arg + 2], (Color) values[arg + 3]);
			break;

		case 'T':
			triangles = generateTriangularPrism(values[arg], values[arg + 1], values[arg + 2], (Color) values[arg + 3]);
			break;
	}

	rotateTriangles(triangles, rotation);
	translateTriangles(triangles, translation);

	return triangles;
}

void computeOperation (char operation, std::vector<Object>& objects, std::stack<size_t>& primitives)
{
	objects.emplace_back();
	Object& combined = objects[objects.size() - 1];

	Object& right = objects[primitives.top()];
	primitives.pop();

	Object& left = objects[primitives.top()];
	primitives.pop();

	switch(operation)
	{
		case '^': // Intersection
		{
			Object::intersection(left, right, combined);
		}
		break;
		case '+': // Union
		{
			Object::unify(left, right, combined);
		}
		break;
		case '-': // Difference
		{
			Object::difference(left, right, combined);
		}
		break;
	}

	primitives.push(objects.size() - 1);
}



void parseFile (std::string file, std::vector<Object>& objects)
{
	std::ifstream text(file);
	std::stringstream buffer;
	buffer << text.rdbuf();
	std::string fileText = buffer.str();
	assert(fileText.size() != 0);

	std::stack<unsigned> operatorOperands;
	std::stack<char> operators;
	std::stack<size_t> primitives;

	unsigned numOfObjects = 0;

	for (unsigned int i = 0; i < fileText.size(); ++i)
	{
		char c = fileText[i];

		if(c == '[')
		{
			++numOfObjects;
		}
	}

	objects.reserve(2 * numOfObjects - 1);

	for (unsigned int i = 0; i < fileText.size(); ++i)
	{
		char c = fileText[i];
		//if empty char then just continue
		switch (c)
		{
			case ' ': // Ignore spaces
				break;

			case '^': // Intersection
			case '+': // Union
			case '-': // Difference
			{
				operatorOperands.push(0);
				operators.push(c);
			}
			break;

			case '[':
			{
				bool found = false;
				int j = i + 1;
				int count = 0;
				while (!found)
				{
					if (fileText[j] != ']')
					{
						++j;
						++count;
					}
					else
					{
						found = true;
					}
				}

				objects.emplace_back(readInPrimitive(fileText.substr(i + 1, count)));
				primitives.push(objects.size() - 1);

				bool validOperator = true;
				while (!operatorOperands.empty() && validOperator)
				{
					unsigned operandCount = operatorOperands.top() + 1;
					operatorOperands.pop();

					assert(operandCount <= 2);
					if (operandCount == 2)
					{
						assert(primitives.size() >= 2);
						computeOperation(operators.top(), objects, primitives);
						operators.pop();
					}
					else
					{
						operatorOperands.push(operandCount);
						validOperator = false;
					}
				}

				fileText = fileText.substr(j + 1);
				i = 0;
			}
			break;

			default:
				break;
		}
	}
}