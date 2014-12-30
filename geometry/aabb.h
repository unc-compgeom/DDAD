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

#ifndef GE_AABB_H
#define GE_AABB_H

#include "common.h"
#include "point.h"
#include "pointset.h"

namespace DDAD {

class AABB_2r {
public:
    AABB_2r();
    AABB_2r(const PointSet_3r& pointset);

    const Point_2r& min() const;
    const Point_2r& max() const;

private:
    Point_2r min_;
    Point_2r max_;
};

} // namespace DDAD

#endif // GE_AABB_H
