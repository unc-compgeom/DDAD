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

#ifndef GE_TRIANGULATION_H
#define GE_TRIANGULATION_H

#include "common.h"
#include "visual.h"
#include "quadedge.h"
#include "matrix.h"

namespace DDAD {

class DelaunayTriangulation_2r : public Visual::Geometry {
public:
    DelaunayTriangulation_2r();
    ~DelaunayTriangulation_2r();

    void Initialize();
    void AddPoint();

private:
    QuadEdge::Cell* triangulation_;
};

} // namespace DDAD

#endif // GE_TRIANGULATION_H
