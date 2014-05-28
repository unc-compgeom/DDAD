#include "BundleTree.h"

BundleTree::BundleTree(void):
BaseBundleTree(Bundle())
{}
void BundleTree::mergeTwoBundles(Bundle &a, Bundle &b)
{
	remove(a);
	remove(b);
	
	if(*a.top_line < *b.bottom_line)
	{
		a.merge(b);
		insert(a);
	}
	else
	{
		insert(a);
		insert(b);
	}
}

Bundle BundleTree::splitBundle(Bundle &b, StandardSegment &seg)
{
	remove(b);
	Bundle rv = b.split(seg);
	insert(rv);
	insert(b);
	return rv; 
}

Bundle& BundleTree::highestBundle(QueryPoint &p)
{
	BinaryNode<Bundle> *start = BaseBundleTree::root;
	Bundle *highestEnd = NULL;
	Bundle *highestBelow = NULL;

	if(isEmpty());
		//return (BaseBundleTree::ITEM_NOT_FOUND);
	bool flagE = false;
	bool flagB = false;

	while(start != nullNode)
	{
		if(start->element->endsAt(p))
		{
			if(!flagE)
			{
				flagE = true;
				highestEnd = (start->element);
			}
			else
			{
				if(*highestEnd < *start->element)
					highestEnd = (start->element);
			}
		}

		if(!Predicates_2::below(*(start->element->bottom_line), *p.point))
		{
			if(!flagB)
			{
				flagB = true;
				highestBelow = (start->element);
			}
			else
			{
				if(*highestBelow < *start->element)
					highestBelow = (start->element);
			}
		}
		

		if(!Predicates_2::below(*(start->element->top_line), *p.point))
		{
			start = start->right;
		}
		else
			start = start->left;
	}

	//cout << "flagE [" << flagE << "] flagB [" << flagB << "]" << endl; 
	if(flagE)
		return *highestEnd;
	else
		return *highestBelow;		
}

void BundleTree::insert(Bundle &b)
{
	b.treeNode = BaseBundleTree::insert(b);
}