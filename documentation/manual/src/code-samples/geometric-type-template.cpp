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
 * @brief Short, high-level description of your file.
 */

#ifndef GE_MY_GEOMETRIC_TYPE_H
#define GE_MY_GEOMETRIC_TYPE_H

#include "common.h"
#include "visual.h"

namespace DDAD {

class MyGeometricType_2r : public Visual::Geometry {
public:
    MyGeometricType_2r();
    MyGeometricType_2r(const MyGeometricType_2r& copy);
    ~MyGeometricType_2r();
}

} // namespace DDAD

#endif // GE_MY_GEOMETRIC_TYPE_H