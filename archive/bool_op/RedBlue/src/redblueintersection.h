#pragma once

#pragma once
#include <iostream>
#include "Bundle.h"
#include "BundleTree.h"
#include "BundleList.h"
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

class RedBlueIntersection
{
public:
	BundleTree redTree;
	BundleList biChromList;
	vector<pair<StandardSegment, StandardSegment>> intersections;
	Node<Bundle> *insert_position;
	QueryPoint previous_point;
	QueryPoint current_point;
	priority_queue<QueryPoint> point_queue;
	Bundle *highest;
	

public:
	RedBlueIntersection()
	{
		insert_position = biChromList.head;
		previous_point = QueryPoint();
		highest = NULL;
	}
	Bundle & locateInRed(QueryPoint &qp);
	Bundle & locateInBlue(Bundle &sRedBundle, QueryPoint &qp, bool direction);
	void splitBundle(Bundle &b, QueryPoint &qp);
	void split_a_red_bundle(Bundle &hBundle, QueryPoint &qp);
	void split_a_blue_bundle(Bundle &hBundle, QueryPoint &qp);
	void split_a_bundle(Bundle &hBundle, QueryPoint &qp);
	void split_after_search(Bundle *highest, QueryPoint &qp);
	void process(QueryPoint &qp);
	bool instersect(Bundle &b1, Bundle &b2);
	void report_intersections(QueryPoint &qp);
	bool intersect(Bundle &b1, Bundle &b2, QueryPoint &qp);
	void merge_neighbors(Node<Bundle> *current); 
	bool merge_bundles(Bundle &upper, Bundle &lower);
	int record_intersections(Bundle &a, Bundle &b);
	void segs_from_bundle(BinaryNode<StandardSegment> *current, BinaryNode<StandardSegment> *null, vector<StandardSegment> &segs);
	void printIntersections();
	void delete_segments(QueryPoint &qp);
	void insert_segments(QueryPoint &qp, Bundle *highest);
	void insert_a_segment(StandardSegment &seg, QueryPoint &qp);
	
	void algorithm()
	{
		read_input("E:\\RB_Input\\redPoly.txt", RED);
		read_input("E:\\RB_Input\\bluePoly.txt", BLUE);
		while(!point_queue.empty())
		{
			cout << "Processing " << point_queue.top().getPoint() << endl;
			current_point = point_queue.top();
			point_queue.pop();

			if(current_point.getPoint() == previous_point.getPoint())
				absorve(current_point);
			else
			{
				process(current_point);
				previous_point = current_point;
				cout << "List: " << endl;
				biChromList.printList();
			}
		}

	}

	void absorve(QueryPoint &qp)
	{
		if(qp.segEnd == RIGHT)//do nothing
			return;
		else
		{
			this->insert_a_segment(*qp.segPointer, qp);
		}
	}

	void read_input(char *fileName, Color seg_color)
	{
		int a, b, c, d;
		ifstream in;
		in.open(fileName);
		StandardSegment * seg = NULL;
		StandardPoint *l = NULL;
		StandardPoint *r = NULL;
		QueryPoint *q = NULL;
		int count = 0;
		while(!in.eof())
		{
			in >> a >> b >> c >> d;
			cout << a << " " << b << " " << c << " " << d << endl;
			l = new StandardPoint(a, b);
			r = new StandardPoint(c, d);
			seg = new StandardSegment(a, b, c, d, seg_color);
			q = new QueryPoint(*l, *seg , LEFT);
			point_queue.push(*q);
			q = new QueryPoint(*r, *seg , RIGHT);
			point_queue.push(*q);
			count+=2;
		}
		cout << count << endl;
		in.close();
	}


	//works now

	void readPolygon(char *fileName, bool outer)
	{
		int a, b, c, d;
		ifstream in;
		in.open(fileName);
		StandardSegment * seg = NULL;
		StandardPoint *l = NULL;
		StandardPoint *r = NULL;
		QueryPoint *q = NULL;
		int count = 0;
		int fa, fb;
		in >> fa >> fb;
		
		while(!in.eof())
		{
			in >> c >> d;
			l = new StandardPoint(fa, fb);
			r = new StandardPoint(c, d);
			if(fa < c)
				seg = new StandardSegment(fa, fb, c, d, RED);
			else
				seg = new StandardSegment(c, d, fa, fb, RED);
			cout << *seg << endl;

			q = new QueryPoint(*r, *seg , LEFT);
			point_queue.push(*q);
			count+=1;
			fa = c;
			fb = d;
		}
		cout << count << endl;
		in.close();
	}


/*	bool isInterior(QueryPoint &qp)
	{
		Color c = qp.getSegPtr().color;
		Color cl, cu;
		if(insert_position)
			cl = insert_position->element->top_line->color;
		else
			return false;
		if(insert_position->next)
			cu = insert_position->next->element->bottom_line->color;
		else
			return false;

		if(c == cl)



	}
*/
	void merge_mono_neighbors(Node<Bundle> *highest);
	~RedBlueIntersection(void){}
};


