/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @brief 3-dimensional polytope types.
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
    ~Polytope_3r();

    void Initialize(const Point_3f& start, const Point_3f& cur);
    void Update(const Point_3f& cur);

    QuadEdge::Cell* cell() const {
        return cell_;
    }
    
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
