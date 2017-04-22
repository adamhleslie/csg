#include "bsp_tree.h"

BspTree::~BspTree ()
{
	if (mFront != nullptr)
	{
		delete mFront;
	}
	if (mBack != nullptr)
	{
		delete mBack;
	}
}

void BspTree::buildTree ()
{
	mFront = new BspTree();
	mBack = new BspTree();
	std::vector<Triangle> onPlane;

	// Categorize each triangle
	Triangle root = getBestTriangle();
	onPlane.push_back(root);

	for (Triangle& triangle : mTriangles)
	{
		root.classifyTriangle(triangle, onPlane, mFront->mTriangles, mBack->mTriangles);
	}

	mTriangles = onPlane;

	if (mFront->mTriangles.empty())
	{
		delete mFront;
		mFront = nullptr;
	}
	else
	{
		mFront->buildTree();
	}

	if (mBack->mTriangles.empty())
	{
		delete mBack;
		mBack = nullptr;
	}
	else
	{
		mBack->buildTree();
	}
}

void BspTree::getTriangles (std::vector<Triangle>& triangles)
{
	triangles.insert(triangles.end(), mTriangles.begin(), mTriangles.end());
	
	if (mFront != nullptr)
	{
		mFront->getTriangles(triangles);
	}
	if (mBack != nullptr)
	{
		mBack->getTriangles(triangles);
	}
}

Triangle BspTree::getBestTriangle ()
{
	Triangle triangle = mTriangles.back();
	mTriangles.pop_back();
	return triangle;
}