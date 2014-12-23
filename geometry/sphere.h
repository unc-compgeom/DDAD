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

#ifndef GE_SPHERE_H
#define GE_SPHERE_H

#include "arithmetic.h"
#include "point.h"

namespace DDAD {

class Sphere_3r  {
public:
    Sphere_3r();
    Sphere_3r(const Point_3r& center, const rational& radius);

    const Point_3r& center() const;
    const rational& radius() const;

private:
    Point_3r center_;
    rational radius_;
};

}

#endif // GE_SPHERE_H
