#pragma once
#include "Point_2.h"
#include "Segment_2.h"
#include "Predicates_2.h"

class Bundle;

enum Color{RED, BLUE, GREEN};
enum END_POINT{LEFT, RIGHT, NONE};
enum Interior {CCW, CW};


typedef Segment_2::Std_segment StdSegment;
typedef Point_2::Std_point StandardPoint;

class StandardSegment : public StdSegment
{

	friend class Bundle;
	
public:
	Color color;
	Interior interior;
	StandardSegment();
	StandardSegment(int x1, int y1, int x2, int y2, Color c);
	StandardSegment(const StandardPoint &pt1, const StandardPoint &pt2, Color c);
	StandardSegment(const StandardPoint &pt1, const StandardPoint &pt2, Color c, Interior side):
	StdSegment(pt1, pt2)
	{
		color = c;
		interior = side;
	}
	

	/***
	*This function checks if ab is below cd by ab < cd. It returns true, 
	*	if
	*	1. c is above ab and d is above ab
	*	OR
	*	2. a is below cd and b is below cd
	*Colinearity is not handled here yet. This code should support this special case too.
	***/

	friend 	bool operator <(const StandardSegment &lhs, const StandardSegment &rhs)
	{
		if(lhs == rhs) return false;
		//both endpoints of rhs is above lhs?
		bool v1 = Predicates_2::above(lhs, rhs.p1_);
		bool v2 = Predicates_2::above(lhs, rhs.p2_);
		bool v3 = v1 && v2;

		//both endpoints of lhs is below rhs?
		bool v4 = !Predicates_2::above(rhs, lhs.p1_);
		bool v5 = !Predicates_2::above(rhs, lhs.p2_);
		bool v6 = v4 && v5;

		//if any of above query returns true, answer is true; false otherwise
		return v3 || v6;
		
	}

	friend 	bool operator ==(const StandardSegment &lhs, const StandardSegment &rhs)
	{
		return lhs.p1_ == rhs.p1_ && lhs.p2_ == rhs.p2_;
		
	}
};

