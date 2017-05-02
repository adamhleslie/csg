#pragma once

#include <vector>
#include "triangle.h"

class BspTree
{
public:
	BspTree () {}
	BspTree (const std::vector<Triangle>& triangles) : mTriangles(triangles) {}

	~BspTree();

	void buildTree();
	void mergeTrees(std::vector<Triangle>& triangles, std::vector<Triangle>* inside, std::vector<Triangle>* outside) const;

	std::vector<Triangle> getTriangles();
	void getTriangles(std::vector<Triangle>& triangles);

private:
	Triangle getBestTriangle();

	std::vector<Triangle> mTriangles;

	BspTree* mFront = nullptr;
	BspTree* mBack = nullptr;
};