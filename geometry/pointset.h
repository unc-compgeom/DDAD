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

#ifndef GE_POINTSET_H
#define GE_POINTSET_H

#include "common.h"
#include "visual.h"
#include "point.h"

namespace DDAD {

//=============================================================================
// Interface: PointSet_3r
//=============================================================================

class PointSet_3r : public Visual::Geometry {
public:
    PointSet_3r();
    ~PointSet_3r();

    void add(const Point_3r& p);
    void add(SharedPoint_3r p);

    SharedPoint_3r operator[](const size_t i) const;
    const size_t size() const;

    const std::vector<SharedPoint_3r>& points() const;
    void set_mat_vertex(const Visual::Material mat_vertex);

private:
    std::vector<SharedPoint_3r> points_;
    Visual::Material mat_vertex_;
};

} // namespace DDAD

#endif // GE_POLYGON_H
