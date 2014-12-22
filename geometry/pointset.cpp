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
#include "pointset.h"

namespace DDAD {

//=============================================================================
// PointSet_3r
//=============================================================================

PointSet_3r::PointSet_3r() {

}

PointSet_3r::~PointSet_3r() {
    for (auto point : points_) {
        SigPopVisualPoint_3r(*point);
    }
}

void PointSet_3r::add(const Point_3r &p) {
    add(std::make_shared<Point_3r>(p.x(), p.y(), p.z()));
}

void PointSet_3r::add(SharedPoint_3r p) {
    points_.push_back(p);

    SigRegisterPoint_3r(*p);
    SigPushVisualPoint_3r(*p, Visual::Point(mat_vertex_));
}

SharedPoint_3r PointSet_3r::operator[](const size_t i) const {
    return points_[i];
}

const size_t PointSet_3r::size() const {
    return points_.size();
}

const std::vector<SharedPoint_3r>& PointSet_3r::points() const {
    return points_;
}

void PointSet_3r::set_mat_vertex(const Visual::Material mat_vertex) {
    mat_vertex_ = mat_vertex;
}

} // namespace DDAD
