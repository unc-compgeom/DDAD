#pragma once
#include "SplayTree.h"
#include "doubly_linked_list.h"
#include "StandardSegment.h"
#include "QueryPoint.h"
#define INFINITY 32000

using namespace std;

class Bundle;
class BundleTree;
class BundleList;
class RedBlueIntersection;
typedef Node<Bundle> LLContainer;




class BundleTree;

typedef SplayTree<StandardSegment> STree;

class Bundle : public STree
{
	StandardSegment *top_line;
	StandardSegment *bottom_line;
	Color color;

	BinaryNode<StandardSegment>* root_(){return this->root;}
	BinaryNode<StandardSegment>* nullNode_(){return this->nullNode;}

	friend class BundleTree;
	friend class BundleList;
	friend class RedBlueIntersection;

	
public:
	Node<Bundle> *listNode;
	BinaryNode<Bundle> *treeNode;

	Bundle(void);


	Bundle(const Bundle& rhs)
	{
		top_line = rhs.top_line;
		bottom_line = rhs.bottom_line;
		color = rhs.color;
		listNode = rhs.listNode;
		treeNode = rhs.treeNode;
	}

	void operator=(const Bundle& rhs)
	{
		top_line = rhs.top_line;
		bottom_line = rhs.bottom_line;
		color = rhs.color;
		listNode = rhs.listNode;
		treeNode = rhs.treeNode;
	}

	Bundle(BinaryNode<StandardSegment> * node): SplayTree(node, StandardSegment(-1, -2, -3, -4, RED )){}
	Bundle(STree &tree, StandardSegment *, StandardSegment *);
	void insert(StandardSegment &s);
	bool contains(StandardSegment &s);
	bool contains(StandardPoint &p);
	bool contains(QueryPoint &p);
	bool endsAt(QueryPoint &p);
	void splitAtPoint(QueryPoint &qp);
	StandardSegment& highestSegmentBelow(QueryPoint &qp);
	StandardSegment& lowestSegmentAbove(QueryPoint &qp);
	
	bool splittable(QueryPoint &qp)
	{
		/**
		This function tests if the current bundle needs 
		to be split using query point p. If p lies on highest
		or lowest segment of the bundle or in between them, then
		the bundle needs to be split.
		*/
		bool b1 = Predicates_2::point_on_line(*top_line, qp.getPoint());
		bool b2 = Predicates_2::point_on_line(*bottom_line, qp.getPoint());
		return contains(qp) && !(b1 && b2);
	}

	Bundle & split_right(QueryPoint &qp)
	{
		StandardSegment *seg = &lowestSegmentAbove(qp);
	}
	/**
	This tree is smaller than rightTree.
	So, always call with a larger tree.
	****/
	void merge(Bundle &rhs);

	/**
	 this will return the left/smaller part of split
	 and modify the current tree to hold the right/bigger part
	*/
	Bundle split(StandardSegment &s);


	friend bool operator==(const Bundle &lhs, const Bundle &rhs)
	{
		/**
		Tests equality of two bundles. If their top_line and bottom_line
		point to the same pair of line segments, then they are the same
		bundle.
		*/
		if(*(rhs.top_line) == *(lhs.top_line) && *(rhs.bottom_line) == *(lhs.bottom_line))
			return true;
		else
			return false;
	}

	friend bool operator != (const Bundle &lhs, const Bundle &rhs)
	{
		return !(lhs==rhs);
	}

	friend bool operator<(const Bundle &lhs, const Bundle &rhs)
	{
		/**
		lhs bundle is smaller than (or lies below) rhs bundle iff top line
		of lhs lies below bottom line of rhs bundle.
		*/
		if(*(lhs.top_line) < *(lhs.bottom_line))
		{
			cout << "Incorrect Order " << *(lhs.top_line) << *(lhs.bottom_line) << endl;
		}

		if(*(rhs.top_line) < *(rhs.bottom_line))
		{
			cout << "Incorrect Order " << *(rhs.top_line) << *(rhs.bottom_line) << endl;
		}

		if(*(lhs.top_line) < *(rhs.bottom_line))
			return true;
		else
			return false;
	}

	friend std::ostream& operator<<(std::ostream& o, const Bundle& b) 
	{

		return o << *(b.bottom_line) << " " << *(b.top_line) << b.color  << endl;
    }


	~Bundle(void);
};



