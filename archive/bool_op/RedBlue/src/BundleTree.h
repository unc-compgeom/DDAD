#pragma once
//#include "SplayTree.h"
#include "Bundle.h"


typedef SplayTree<Bundle> BaseBundleTree;
class BundleTree : public BaseBundleTree
{
	friend class RedBlueIntersection;
public:
	BundleTree();
	void mergeTwoBundles(Bundle &a, Bundle &b);
	Bundle splitBundle(Bundle &b, StandardSegment &seg);
	void insert(Bundle &b);
	Bundle & highestBundle(QueryPoint &p);
};
