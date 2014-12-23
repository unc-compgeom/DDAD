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

#include "sphere.h"

namespace DDAD {

Sphere_3r::Sphere_3r() :
    center_(0, 0, 0),
    radius_(0) {}

Sphere_3r::Sphere_3r(const Point_3r &center, const rational &radius) :
    center_(center),
    radius_(radius) {}

const Point_3r& Sphere_3r::center() const {
    return center_;
}
const rational& Sphere_3r::radius() const {
    return radius_;
}

} // namespace DDAD
