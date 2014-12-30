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
#include "aabb.h"

namespace DDAD {

AABB_2r::AABB_2r() {}

AABB_2r::AABB_2r(const PointSet_3r &pointset) {
    rational minx, miny, maxx, maxy;
    minx = maxx = pointset.points()[0]->x();
    miny = maxy = pointset.points()[0]->y();

    for (auto point : pointset.points()) {
        minx = std::min(point->x(), minx);
        miny = std::min(point->y(), miny);
        maxx = std::max(point->x(), maxx);
        maxy = std::max(point->y(), maxy);
    }

    min_.set_x(minx);
    min_.set_y(miny);
    max_.set_x(maxx);
    max_.set_y(maxy);
}

const Point_2r& AABB_2r::min() const {
    return min_;
}

const Point_2r& AABB_2r::max() const {
    return max_;
}

} // namespace DDAD
