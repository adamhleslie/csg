#include "bsp_tree.h"

void BspTree::buildTree ()
{
	mFront = new BspTree();
	mBack = new BspTree();

	Triangle triangle = getBestTriangle();

}

Triangle BspTree::getBestTriangle()
{
	Triangle triangle = *(mTriangles.back());
	mTriangles.pop_back();
	
}