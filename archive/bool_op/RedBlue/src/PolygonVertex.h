#pragma once
#include "StandardSegment.h"

class PolygonalChain;

class PolygonVertex
{
	StandardSegment *left;
	StandardSegment *right;
	StandardPoint *witness;
	friend class PolygonalChain;
public:
	PolygonVertex(void);
	~PolygonVertex(void);
};
