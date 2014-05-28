#pragma once
#include "RedBlueIntersection.h"
#include <iostream>
#include "Bundle.h"

using namespace std;

Bundle & RedBlueIntersection::locateInRed(QueryPoint &qp)
{
	return redTree.highestBundle(qp);
}

Bundle & RedBlueIntersection::locateInBlue(Bundle &sRedBundle, QueryPoint &qp, bool direction)
{
	Node<Bundle>* current = sRedBundle.listNode;
	return biChromList.highestBlueBundle(sRedBundle, qp, direction);
}

void RedBlueIntersection::splitBundle(Bundle &b, QueryPoint &qp)
{
	//Bundle lBundle(b.split(b.highestSegmentBelow(qp)));
	//Bundle h;
	//h.root = NULL;
	//Bundle h = b.split_right(b.lowestSegmentAbove(qp), StandardSegment(-1, -2, -3, -4, RED ));

}

void RedBlueIntersection::split_a_red_bundle(Bundle &hBundle, QueryPoint &qp)
{
	bool changeHighest = (*highest == hBundle);
	StandardSegment notFound(-1, -2, -3, -4, GREEN);
	if(hBundle.color != RED)
	{
		cout << "Color Invariant is Broken " << endl;
		return;
	}
	/***
	Split the Bundle in at most 3 parts, 
	then insert them in the redTree in place of the original bundle
	****/
	BinaryNode<Bundle> *current = hBundle.treeNode;
	StandardSegment *h = &(current->element->lowestSegmentAbove(qp));
	StandardSegment *l = &(current->element->highestSegmentBelow(qp));
	if(*l == notFound)l = NULL;
	if(*h == notFound)h = NULL;

	StandardSegment *top = hBundle.top_line;
	StandardSegment *bottom = hBundle.bottom_line;

	BinaryNode<StandardSegment> * temp = NULL;
	redTree.remove(hBundle);


	//if h == NULL do nothing
	if(h)
	{
		hBundle.find(*h);
		temp = hBundle.root->left;
		hBundle.root->left = hBundle.nullNode;
		hBundle.bottom_line = h;
	}

	redTree.insert(hBundle);

	Bundle *lower = NULL;
	
	if(l)
	{
		lower = new Bundle(temp);
		lower->nullNode = hBundle.nullNode;
		lower->find(*l);
		temp = lower->root->right;
		lower->root->right = lower->nullNode;
		lower->top_line = l;	
		lower->bottom_line = bottom;
		lower->color = RED;
		redTree.insert(*lower);
	}


	Bundle * ending;

	if(temp != hBundle.nullNode)
	{
		ending = new Bundle(temp);
		ending->nullNode = hBundle.nullNode;
		ending->findMax();
		ending->top_line = ending->root->element;
		ending->findMin();
		ending->bottom_line = ending->root->element;
		ending->color = RED;
		redTree.insert(*ending);
	}


	/****
	Insert the newly formed Bundels into bundle list and remove the original one
	****/
	Node<Bundle> *listPointer = hBundle.listNode->prev;
	biChromList.remove(listPointer->next);

	if(lower)
	{
		biChromList.insert_after(listPointer, *lower);
		listPointer = listPointer->next;
	}
	if(ending)
	{
		biChromList.insert_after(listPointer, *ending);
		listPointer = listPointer->next;
	}
	if(h)biChromList.insert_after(listPointer, hBundle);

	if(ending)insert_position = ending->listNode;
	else if(lower)insert_position = lower->listNode;

	 
	if(changeHighest)
	{
		if(h)highest = &hBundle;
		else if(ending)highest = ending;
		else highest = lower;
	}


}

//b1 preceeds b2
//b1->prev = b2
bool RedBlueIntersection::intersect(Bundle &b1, Bundle &b2, QueryPoint &qp)
{
	if(b1.color == b2.color)return false;
	bool b1_ending = Predicates_2::point_on_line(*b1.bottom_line, qp.getPoint());
	bool b1_below = Predicates_2::above(*b1.top_line, qp.getPoint());
	bool b2_ending = Predicates_2::point_on_line(*b2.bottom_line, qp.getPoint());
	bool b2_above = Predicates_2::below(*b2.bottom_line, qp.getPoint());

	return (b2_above && (b1_ending || b1_below)) || (b2_ending && (b1_ending || b1_below));
}

void RedBlueIntersection::split_a_blue_bundle(Bundle &hBundle, QueryPoint &qp)
{
	bool changeHighest = (*highest == hBundle);
	StandardSegment notFound(-1, -2, -3, -4, GREEN);
	if(hBundle.color != BLUE)
	{
		cout << "Called function with wrong color " << endl;
		return;
	}
	/***
	Split the Bundle in at most 3 parts
	****/

	Node<Bundle> *current = hBundle.listNode;
	StandardSegment *h = &(current->element->lowestSegmentAbove(qp));
	StandardSegment *l = &(current->element->highestSegmentBelow(qp));
	if(*l == notFound)l = NULL;
	if(*h == notFound)h = NULL;
	StandardSegment *top = hBundle.top_line;
	StandardSegment *bottom = hBundle.bottom_line;

	BinaryNode<StandardSegment> * temp = NULL;

	//if h == NULL do nothing
	if(h)
	{
		hBundle.find(*h);
		temp = hBundle.root->left;
		hBundle.root->left = hBundle.nullNode;
		hBundle.bottom_line = h;
	}

	else
	{
		temp = hBundle.root;
	}

	Bundle *lower = NULL;
	if(l)
	{
		lower = new Bundle(temp);
		lower->nullNode = hBundle.nullNode;
		lower->find(*l);
		temp = lower->root->right;
		lower->root->right = lower->nullNode;
		lower->top_line = l;	
		lower->bottom_line = bottom;
		lower->color = BLUE;
	}

	
	Bundle * ending = NULL;
	if(lower)
	if(temp != lower->nullNode)
	{
		ending = new Bundle(temp);
		ending->nullNode = lower->nullNode;
		ending->findMax();
		ending->top_line = ending->root->element;
		ending->findMin();
		ending->bottom_line = ending->root->element;
		ending->color = BLUE;
	}


	/****
	Insert the newly formed Bundels into bundle list and remove the original one
	****/


	Node<Bundle> *listPointer = NULL;
	if(hBundle.listNode)
		listPointer = hBundle.listNode->prev;

	biChromList.remove(hBundle.listNode);

	if(lower)
	{
		if(listPointer)
		{
			biChromList.insert_after(listPointer, *lower);
		}
		else
			biChromList.insert_at_front(*lower);
		listPointer = lower->listNode;
	}
	if(ending)
	{
		biChromList.insert_after(listPointer, *ending);
		listPointer = ending->listNode;
	}
	if(h)biChromList.insert_after(listPointer, hBundle);
	if(ending)insert_position = ending->listNode;
	else if(lower)insert_position = lower->listNode;

	if(changeHighest)
	{
		if(h)highest = &hBundle;
		else if(ending)highest = ending;
		else highest = lower;
	}

}

void RedBlueIntersection::split_a_bundle(Bundle &hBundle, QueryPoint &qp)
{
	if(hBundle.color == RED)
		split_a_red_bundle(hBundle, qp);
	else
		split_a_blue_bundle(hBundle, qp);
}

void RedBlueIntersection::merge_neighbors(Node<Bundle> *current)
{
	Node<Bundle> *temp = NULL;
	if(current) temp = current->prev;
	
	if(current->prev)
	if(merge_bundles(*current->element, *current->prev->element))
	{
		current = temp;
		temp = NULL;
	}
	if(current->next->next)
	merge_bundles(*current->next->next->element, *current->next->element);
}

void RedBlueIntersection::segs_from_bundle(BinaryNode<StandardSegment> *current, BinaryNode<StandardSegment> *null, vector<StandardSegment> &segs)
{
	if(current != current->left)
	{
		segs_from_bundle(current->left, null, segs);
		segs.push_back(*current->element);
		segs_from_bundle(current->right, null, segs);
	}
}

int RedBlueIntersection::record_intersections(Bundle &a, Bundle &b)
{
	BinaryNode<StandardSegment> *a_ptr = a.root;
	BinaryNode<StandardSegment> *b_ptr = b.root;

	a.root;
	Bundle temp_a = a;
	Bundle temp_b = b;
	vector<StandardSegment> a_segs;
	vector<StandardSegment> b_segs;

	this->segs_from_bundle(a_ptr, a.nullNode, a_segs);
	this->segs_from_bundle(b_ptr, b.nullNode, b_segs);

	for(vector<StandardSegment>::iterator it = a_segs.begin(); it != a_segs.end(); ++it)
	{
		for(vector<StandardSegment>::iterator jt = b_segs.begin(); jt != b_segs.end(); ++jt) 
		{
			intersections.push_back(make_pair(*it, *jt));
		}
	}

	return 0;
}

bool RedBlueIntersection::merge_bundles(Bundle &upper, Bundle &lower)
{
	Node<Bundle> *prev_ptr = lower.listNode->prev;
	if(upper.color == lower.color)
	{
		if(upper.color == RED){
			redTree.remove(upper);
			redTree.remove(lower);
		}

		biChromList.remove(upper.listNode);
		biChromList.remove(lower.listNode);
		lower.merge(upper);

		lower.listNode->prev = prev_ptr;
		prev_ptr = NULL;
		if(upper.color == RED)
		{
			redTree.insert(lower);
		}

		if(lower.listNode->prev)
			biChromList.insert_after(lower.listNode->prev, lower);
		else
			biChromList.insert_at_front(lower);
		return true;
	}
	return false;
}

void RedBlueIntersection::report_intersections(QueryPoint &qp)
{
	if(highest == NULL)return;

	Node<Bundle> *current = highest->listNode;

	while(current->prev)
	{
		if(intersect(*current->element, *current->prev->element, qp))
		{
			cout << *current->element << " Intersects with " << *current->prev->element << endl;
			this->record_intersections(*current->element, *current->prev->element);
			if((highest)->listNode == current)
				highest = (highest)->listNode->prev->element;
			biChromList.swap_two_nodes(current->prev, current);	
			if(current->next->next)
			if(intersect(*current->next->next->element, *current->next->element, qp))
				current = current->next->next;
			//merge_neighbors(current);
		}
		else if(current->element->color != current->element->color){
			cout << "breaking the loop " << endl;
			break;
		}
		else
			current = current->prev;
	}

}

void RedBlueIntersection::process(QueryPoint &qp)
{
	insert_position = NULL;
	Bundle *redHighest = NULL;
	Bundle *blueHighest = NULL;
	Node<Bundle>* current = NULL;

	if(!redTree.isEmpty())
	{
		redHighest = &locateInRed(qp);
		if(redHighest)
			current = redHighest->listNode;
		else
		{
			if(biChromList.head->element->color == BLUE)
			current = biChromList.head;
		}
	}

	bool direction = false;
	if(current && current->next)
		direction = !Predicates_2::below(*(current->next->element->bottom_line), qp.getPoint());
	else
		direction = false;

	
	if(!biChromList.is_empty())
	{
		if(redHighest)
		blueHighest = &locateInBlue(*redHighest, qp, direction);
		else
		{
			if(biChromList.head->element->color == BLUE)
				if(!Predicates_2::below(*biChromList.head->element->bottom_line, qp.getPoint()))
				blueHighest = biChromList.head->element;
		}
	}

	highest = NULL;
	if(redHighest)
		highest = (direction) ? blueHighest : redHighest;
	else
		highest = blueHighest;

	if(highest)insert_position = highest->listNode;
	if(highest)split_after_search(highest, qp);

	report_intersections(qp);


	if(qp.segEnd == RIGHT)delete_segments(qp);
	if(qp.segEnd == LEFT)insert_a_segment(*qp.segPointer, qp);
	if(highest)merge_mono_neighbors(highest->listNode);

}

void RedBlueIntersection::split_after_search(Bundle *highest, QueryPoint &qp)
{
	bool red_on = true;
	bool blue_on = true;

	Node<Bundle> *current = highest->listNode;
	Node<Bundle> *previous = current->prev;
	int count = 0;
	while(current && (red_on || blue_on))
	{
		if(current->element->splittable(qp))
		{
			split_a_bundle(*current->element, qp);			
		}
		else if(Predicates_2::above(*current->element->top_line, *qp.point))
		{
			if(current->element->color == RED)
				red_on = false;
			else
				blue_on = false;
		}
		current = previous;
		if(previous == NULL)continue;
		previous = previous->prev;
	}
}

void RedBlueIntersection::printIntersections()
{
	for(std::vector<pair<StandardSegment, StandardSegment>>::iterator it
		= intersections.begin(); it != intersections.end(); ++it) 
	{
		cout << it->first << " X " << it->second << endl;
	}
}

void RedBlueIntersection::delete_segments(QueryPoint &qp)
{
	Node<Bundle> *current =  highest->listNode;
	Node<Bundle> *lower = NULL;
	Node<Bundle> *higher = NULL;
	Node<Bundle> *last_above = NULL;
	Node<Bundle> *first_below = NULL;
	
	//cout << *current->element << endl;
	bool red_on = true, blue_on = true;
	while(current && (red_on || blue_on))
	{
		if(first_below == NULL)
		if(Predicates_2::above(*current->element->top_line, qp.getPoint()))first_below = current;
		if(Predicates_2::below(*current->element->bottom_line, qp.getPoint()))last_above = current;

		if(current->element->endsAt(qp))
		{
			(current->element->color == RED) ? (red_on = false) : (blue_on = false);
			lower = current->prev;
			higher = current->next;
			if(higher)highest = higher->element;
			else if(lower)highest = lower->element;
			if(current->element->color == RED)
				redTree.remove(*current->element);
			biChromList.remove(current);
			current = lower;
		}
		else
		current = current->prev;
	}
	if(first_below)
	insert_position = first_below;
}


void RedBlueIntersection::insert_segments(QueryPoint &qp, Bundle *insert_place)
{

	if(insert_place == NULL)//insert into front
	{
		
	}
	Node<Bundle> *current = insert_place->listNode;

	if(qp.segEnd == RIGHT)
	{
		
	}
}

void RedBlueIntersection::insert_a_segment(StandardSegment &seg, QueryPoint &qp)
{
	StandardSegment *notFound = new StandardSegment(-1, -2, -3, -4, seg.color);
	Bundle *newBundle = NULL;


	if(!biChromList.is_empty())
	{
		if(insert_position){
			if(insert_position->element->color == seg.color)
				insert_position->element->insert(seg);
			else if(insert_position->next->element->color == seg.color)
				insert_position->next->element->insert(seg);
			else
			{
				newBundle = new Bundle();
				newBundle->insert(seg);
				biChromList.insert_after(insert_position, *newBundle);
				if(seg.color == RED)
					redTree.insert(*newBundle);
			}
		}
		else
		{
			if(seg.color == biChromList.head->element->color)
				biChromList.head->element->insert(seg);
			else
			{
				newBundle = new Bundle();
				newBundle->insert(seg);
				biChromList.insert_at_front(*newBundle);
				if(seg.color == RED)
					redTree.insert(*newBundle);
			}
		}
	}
	else
	{
		newBundle = new Bundle();
		newBundle->insert(seg);
		biChromList.insert_after(insert_position, *newBundle);
		if(seg.color == RED)
			redTree.insert(*newBundle);
	}


	
}


void RedBlueIntersection :: merge_mono_neighbors(Node<Bundle> *highest)
{
	Node<Bundle> *current = highest;
	Node<Bundle> *previous = current;
	if(highest)
	{
		while(current)
		{
			if(current->prev)
				if(current->element->color == current->prev->element->color)
				{
					previous = current->prev;
					this->merge_bundles(*current->element, *current->prev->element);
					current = previous;
				}
				else current = current->prev;
			else
				current = current->prev;

		}
	}
}