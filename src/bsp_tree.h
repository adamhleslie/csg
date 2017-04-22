#pragma once

#include <vector>
#include "triangle.h"

class BspTree
{
public:
	BspTree () {}
	BspTree (std::vector<Triangle>& triangles) : mTriangles(triangles) {}

	~BspTree();

	void buildTree();
	void getTriangles(std::vector<Triangle>& triangles);

private:
	Triangle getBestTriangle();

	std::vector<Triangle> mTriangles;

	BspTree* mFront = nullptr;
	BspTree* mBack = nullptr;
};