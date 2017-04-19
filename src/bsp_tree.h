#pragma once

#include "triangle.h"

class BspTree
{
public:
	BspTree() {}
	BspTree(std::vector<Triangle>& triangles) : mTriangles(triangles) {}

	void buildTree();

private:
	Triangle getBestTriangle();

	std::vector<Triangle> mTriangles;

	BspTree* mFront;
	BspTree* mBack;
};