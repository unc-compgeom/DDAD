#include "Bundle.h"





Bundle::Bundle(void):
STree(StandardSegment(-1, -2, -3, -4, RED ))
{
	top_line = NULL;
	bottom_line = NULL;
	color = GREEN;
	listNode = NULL;
	treeNode = NULL;
}

Bundle::Bundle(STree &tree, StandardSegment *top, StandardSegment *bottom):STree(tree)
{
	top_line = top;
	bottom_line = bottom;
	listNode = NULL;
	treeNode = NULL;
}


bool Bundle::contains(StandardSegment &s)
{
	/**
	This method tests if a segment s is in current tree.
	*/
	if(s == find(s))
		return true;
	else 
		return false;
}
	
bool Bundle::contains(StandardPoint &p)
{
	/**
	This method tests if point p is below top line and above bottom line.
	*/
	return Predicates_2::above(*bottom_line, p) && 
		!Predicates_2::above(*top_line, p);
}

bool Bundle::contains(QueryPoint &p)
{
	return !Predicates_2::below(*bottom_line, *p.point) && 
		!Predicates_2::above(*top_line, *p.point);
}

bool Bundle::endsAt(QueryPoint &p)
{

	/**
	A search is conducted to see if any segment in current tree ends at point p.
	above(segment s, point p) test is used to search through a tree with segments.
	*/
	if(Predicates_2::point_on_line(*top_line, *p.point) || Predicates_2::point_on_line(*bottom_line, *p.point))
		return true;
	if(!contains(p))
		return false;
	BinaryNode<StandardSegment> *current = root;
	while(current != nullNode)
	{
		if(Predicates_2::point_on_line(*current->element, *p.point))
			return true;
		if(Predicates_2::above(*current->element, *p.point))
			current = current->right;
		else if(Predicates_2::below(*current->element, *p.point))
			current = current->left;
	}

	if(find(*p.segPointer) == ITEM_NOT_FOUND)
		return false;
	else
		return true;
}

/****
*** this tree is smaller than rightTree
*** so, always call with a larger tree
****/
void Bundle::merge(Bundle &rhs)
{
	//Node<Bundle> *prev_ptr = this->listNode;
	if( *top_line < *rhs.bottom_line)
	{
		STree::merge(rhs);
		top_line = rhs.top_line;
		//this->listNode = prev_ptr;
		//prev_ptr = NULL;
	}
	else
	{
		cout << "Cannot merge a smaller bundle." << endl;
	}
}


Bundle Bundle::split(StandardSegment &s)
{
	/**
	This will return the left/smaller part of split
	and modify the current tree to hold the right/bigger part
	*/
	Bundle rv = Bundle(STree::split(s, STree::ITEM_NOT_FOUND), NULL, bottom_line);
	rv.top_line = (StandardSegment*)&rv.findMax();
	bottom_line = (STree::root->element);
	return rv;
}

void Bundle::insert(StandardSegment &s)
{
	/** Segment s is inserted into current bundle if
	s is below top line and above bottom line. s is also
	tested for color, so that segment of wrong color is not
	inserted into current bundle.
	*/
	if(color != GREEN && color != s.color)
		return;
	

	if(top_line == NULL && bottom_line == NULL)
	{
		top_line = bottom_line = &s;
		color = s.color;
	}
	else 
	{
		if(*top_line < s)
			top_line = &s;
		else
			if(s < *bottom_line)
			bottom_line = &s;
	}

	//cout << "Update Top and Bottom "  << *top_line << *bottom_line << endl;

	STree::insert(s);
	if(*top_line < *bottom_line) cout << "Insertion was not successfull " << endl;
}


StandardSegment& Bundle::highestSegmentBelow(QueryPoint &qp)
{
	/**
	@
	*/
	BinaryNode<StandardSegment> *current = root;
	BinaryNode<StandardSegment> *LIB = NULL;
	BinaryNode<StandardSegment> *LRB = NULL;

	while(current != nullNode)
	{
		if(Predicates_2::below(*current->element, qp.getPoint()))
		{
			LIB = current;
			current = current->left;
		}
		else if(Predicates_2::above(*current->element, qp.getPoint()))
		{
			LRB = current;
			current = current->right;
		}
		else
			current = current->left;
	}

	if(!LRB)
		return ITEM_NOT_FOUND;
	return *LRB->element;

}

StandardSegment& Bundle::lowestSegmentAbove(QueryPoint &qp)
{
	BinaryNode<StandardSegment> *current = root;
	BinaryNode<StandardSegment> *LIB = NULL;
	BinaryNode<StandardSegment> *LRB = NULL;

	while(current != nullNode)
	{
		if(Predicates_2::below(*current->element, qp.getPoint()))
		{
			LIB = current;
			current = current->left;
		}
		else if(Predicates_2::above(*current->element, qp.getPoint()))
		{
			LRB = current;
			current = current->right;
		}
		else
		{
			current = current->right;
		}
	}
	StandardSegment *dummy = new StandardSegment(-1, -2, -3, -4, GREEN);
	if(!LIB)
		return ITEM_NOT_FOUND;
	return *LIB->element;
}

void Bundle::splitAtPoint(QueryPoint &qp)
{

}
Bundle::~Bundle(void)
{

}
