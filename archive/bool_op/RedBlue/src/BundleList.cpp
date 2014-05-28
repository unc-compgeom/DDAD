#include "BundleList.h"

Bundle& BundleList::highestInRight(Node<Bundle> *sBlueNode, QueryPoint &qp)
{
	Node<Bundle> *current = sBlueNode;
	
	while(current)
	{
		if(Predicates_2::above(*(current->element->top_line), qp.getPoint()))
		{
			//testing below, but this test will also return true if the point lies on the segment
			//should have a below function
			if(current->next){

				if(current->next->next){
					if(Predicates_2::above(*(current->next->next->element->bottom_line), qp.getPoint()))
					{
						current = current->next->next;
					}
					else break;
				}
				else break;
			}
			else break;
		}
		else break;
	}
	return *current->element;
}




Bundle& BundleList::highestInLeft(Node<Bundle> *sBlueNode, QueryPoint &qp)
{
	
	Node<Bundle> *current = sBlueNode;
	
	while(current)
	{
		if(!Predicates_2::above(*(current->element->bottom_line), qp.getPoint()))
		{
			//testing below, but this test will also return true if the point lies on the segment
			//should have a below function
			if(current->prev){
				if(current->prev->prev){
					if(Predicates_2::below(*(current->prev->prev->element->top_line), qp.getPoint()))
					{
						current = current->prev->prev;
					}
					else
					{
						current = current->prev->prev;
						break;
					}
				}
				else break;
			}
			else break;
		}
		else break;
		
	}
	if(current)
		return *current->element;
}

Bundle& BundleList::highestBlueBundle(Bundle &sBundle, QueryPoint &qp, bool direction)
{
	/**
	Input: sBundle
	*/
	Node<Bundle>* current = sBundle.listNode;
	if(direction)
		return highestInRight(current->next, qp);
	else
		return highestInLeft(current->prev, qp);
}

void BundleList::insert_after(LLContainer *prev, Bundle &b)
{
	b.listNode = BaseBundleList::insert_after(prev, b);
}

void BundleList::insert_first_element(Bundle &b)
{
	b.listNode = BaseBundleList::insert_first_element(b);
}

void BundleList::insert_at_front(Bundle &b)
{
	b.listNode = BaseBundleList::insert_at_front(b);
}
