#pragma once
#include "Bundle.h"

typedef LinkedList<Bundle> BaseBundleList;
class BundleList : public BaseBundleList
{
	friend class RedBlueIntersection;
public:
	BundleList():BaseBundleList(){};
	BundleList(Node<Bundle> *h, Node<Bundle> *t):BaseBundleList(h, t){}
	Bundle &highestInRight(Node<Bundle> *sBlueNode, QueryPoint &qp);
	Bundle &highestInLeft(Node<Bundle> *sBlueNode, QueryPoint &qp);
	Bundle &highestBlueBundle(Bundle &sBundle, QueryPoint &qp, bool direction);
	void split_in_blue(Bundle &sBundle);
	void split_in_red(Bundle &sBundle);
	void insert_before(LLContainer *next, Bundle &b);
	void insert_after(LLContainer *prev, Bundle &b);
	void insert_first_element(Bundle &b);
	void insert_at_front(Bundle &b);
};
