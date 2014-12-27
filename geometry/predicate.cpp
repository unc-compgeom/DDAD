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

#include "common.h"
#include "point.h"
#include "matrix.h"
#include "predicate.h"

namespace DDAD {

namespace Predicate {

Orientation OrientationPQR(const Point_2i &p, const Point_2i &q,
                           const Point_2i &r) {
    rational det = Determinant(Matrix_2x2r(q.x()-p.x(), q.y()-p.y(),
                                           r.x()-p.x(), r.y()-p.y()));

    if (det > 0) {
        return ORIENTATION_LEFT;
    } else if (det < 0) {
        return ORIENTATION_RIGHT;
    } else {
        return ORIENTATION_COLINEAR;
    }
}

Orientation OrientationPQR(const Point_2r &p, const Point_2r &q,
                           const Point_2r &r) {
    rational det = Determinant(Matrix_2x2r(q.x()-p.x(), q.y()-p.y(),
                                           r.x()-p.x(), r.y()-p.y()));

    if (det > 0) {
        return ORIENTATION_LEFT;
    } else if (det < 0) {
        return ORIENTATION_RIGHT;
    } else {
        return ORIENTATION_COLINEAR;
    }
}

Orientation OrientationPQR(const Point_2f &p, const Point_2f &q,
                           const Point_2f &r) {
    rational det = Determinant(Matrix_2x2r(q.x()-p.x(), q.y()-p.y(),
                                           r.x()-p.x(), r.y()-p.y()));

    if (det > 0) {
        return ORIENTATION_LEFT;
    } else if (det < 0) {
        return ORIENTATION_RIGHT;
    } else {
        return ORIENTATION_COLINEAR;
    }
}

bool RIsLeftOrInsidePQ(const Point_2r &p, const Point_2r &q,
                       const Point_2r &r) {
    Orientation orientation = OrientationPQR(p, q, r);

    bool is_left = orientation == ORIENTATION_LEFT;

    auto v0 = q-p;
    auto v1 = r-p;
    auto dist = Dot(v0, v1);
    //LOG(INFO) << v0 << v1 << dist;

    bool is_inside = orientation == ORIENTATION_COLINEAR && dist >= 0 && dist <= Dot(v0, v0);

    return is_left || is_inside;
}

} // namespace Predicate

} // namespace DDAD
