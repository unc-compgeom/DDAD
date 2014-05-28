#pragma once
#include "StandardSegment.h"

class QueryPoint
{
	StandardPoint *point;
	StandardSegment *segPointer;
	END_POINT segEnd;
	friend class Bundle;
	friend class BundleTree;
	friend class BundleList;
	friend class RedBlueIntersection;
public:
	QueryPoint()
	{
		point = new StandardPoint(-99999, -99999);
		segPointer = new StandardSegment(-1, -2, -3, -4, GREEN);
		segEnd = NONE;
	}
	QueryPoint(StandardPoint &pt, StandardSegment &seg, END_POINT sEnd)
	{
		point = &pt;
		segPointer = &seg;
		segEnd = sEnd;
	}

	friend bool operator<(const QueryPoint &lhs, const QueryPoint &rhs)
	{
		if(*rhs.point == *lhs.point)
		{
			if(rhs.segEnd == RIGHT && lhs.segEnd == LEFT)return false;
			if(rhs.segEnd == LEFT && lhs.segEnd == RIGHT)return true;
		}
		return *rhs.point < *lhs.point;
	}

	StandardPoint& getPoint(){return *point;}
	StandardSegment& getSegPtr(){return *segPointer;}
	END_POINT getEnd(){return segEnd;}
};
