#pragma once

#include <vector>
#include <algorithm>
#include "triangle.h"

class BspTree
{
public:
	BspTree () {}
	BspTree(const BspTree& other);
	BspTree& operator=(const BspTree& other);
	BspTree (const std::vector<Triangle>& triangles) : mTriangles(triangles) {}

	~BspTree();

	void buildTree();
	void mergeTrees(std::vector<Triangle> triangles, std::vector<Triangle>* inside, std::vector<Triangle>* outside, bool onAsFront = true) const;

	std::vector<Triangle> getTriangles();
	void getTriangles(std::vector<Triangle>& triangles);

private:
	Triangle getBestTriangle();
	void clone (const BspTree* other);

	std::vector<Triangle> mTriangles;

	BspTree* mFront = nullptr;
	BspTree* mBack = nullptr;
};