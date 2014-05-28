/*!
 * @brief 3-dimensional polytope types.
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-10-23
 */

#ifndef GE_POLYTOPE_H
#define GE_POLYTOPE_H

#include "common.h"
#include "visual.h"
#include "point.h"
#include "line.h"
#include "triangle.h"

namespace DDAD {

namespace QuadEdge {
class Cell;
}

//=============================================================================
// Interface: Polytope_3r
//=============================================================================

class Polytope_3r : public Visual::Geometry {
public:
    Polytope_3r();

    void Initialize(const Point_3f& start, const Point_3f& cur);
    void Update(const Point_3f& cur);
    
private:
    QuadEdge::Cell* cell_;
    Point_3f start_;
};

namespace Construction {

Polytope_3r ConvexHull(const std::vector<SharedPoint_3r>& P);
Polytope_3r IntegerHull(const Polytope_3r& P);

}

//=============================================================================
// Interface: Tetrahedron_3r
//=============================================================================

/*
class Tetrahedron_3r : public Visual::Geometry {
public:
    Tetrahedron_3r();

private:
    SharedPoint_3r a_;
    SharedPoint_3r b_;
    SharedPoint_3r c_;
    SharedPoint_3r d_;
};
*/

} // namespace DDAD

#endif // GE_POLYTOPE_H
