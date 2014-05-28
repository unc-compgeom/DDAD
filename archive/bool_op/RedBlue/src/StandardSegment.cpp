#include "StandardSegment.h"

StandardSegment::StandardSegment():
StdSegment(StandardPoint(-1, -2), StandardPoint(-3, -4))
{
	color = GREEN;
}

StandardSegment::StandardSegment(int x1, int y1, int x2, int y2, Color c):
StdSegment(StandardPoint(x1, y1), StandardPoint(x2, y2))
{
	color = c;
}

StandardSegment::StandardSegment(const StandardPoint &pt1, const StandardPoint &pt2, Color c):
StdSegment(pt1, pt2)
{
	color = c;
}
