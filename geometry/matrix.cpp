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
#include "matrix.h"
#include "point.h"
#include "vector.h"

namespace DDAD {

//=============================================================================
// Implementation: Matrix_3x3i
//=============================================================================

integer Determinant(const Matrix_3x3i& mat) {
    return mat(0,0)*mat(1,1)*mat(2,2) -
           mat(0,0)*mat(2,1)*mat(1,2) +
           mat(1,0)*mat(2,1)*mat(0,2) -
           mat(1,0)*mat(0,1)*mat(2,2) +
           mat(2,0)*mat(0,1)*mat(1,2) -
           mat(2,0)*mat(1,1)*mat(0,2);
}

Matrix_3x3i Inverse(const Matrix_3x3i& mat) {
    integer d = Determinant(mat);
    return Matrix_3x3i( (mat(1,1)*mat(2,2)-mat(1,2)*mat(2,1))/d,
                       -(mat(0,1)*mat(2,2)-mat(0,2)*mat(2,1))/d,
                        (mat(0,1)*mat(1,2)-mat(0,2)*mat(1,1))/d,
                       -(mat(1,0)*mat(2,2)-mat(1,2)*mat(2,0))/d,
                        (mat(0,0)*mat(2,2)-mat(0,2)*mat(2,0))/d,
                       -(mat(0,0)*mat(1,2)-mat(0,2)*mat(1,0))/d,
                        (mat(1,0)*mat(2,1)-mat(1,1)*mat(2,0))/d,
                       -(mat(0,0)*mat(2,1)-mat(0,1)*mat(2,0))/d,
                        (mat(0,0)*mat(1,1)-mat(0,1)*mat(1,0))/d);
}

//=============================================================================
// Implementation: Matrix_3x3r
//=============================================================================

rational Determinant(const Matrix_3x3r& mat) {
    return mat(0,0)*mat(1,1)*mat(2,2) -
           mat(0,0)*mat(2,1)*mat(1,2) +
           mat(1,0)*mat(2,1)*mat(0,2) -
           mat(1,0)*mat(0,1)*mat(2,2) +
           mat(2,0)*mat(0,1)*mat(1,2) -
           mat(2,0)*mat(1,1)*mat(0,2);
}

Matrix_3x3r Inverse(const Matrix_3x3r& mat) {
    rational d = Determinant(mat);
    d = (d == 0) ? 1 : rational(d.get_den(), d.get_num());

    return Matrix_3x3r( (mat(1,1)*mat(2,2)-mat(1,2)*mat(2,1))*d,
                       -(mat(0,1)*mat(2,2)-mat(0,2)*mat(2,1))*d,
                        (mat(0,1)*mat(1,2)-mat(0,2)*mat(1,1))*d,
                       -(mat(1,0)*mat(2,2)-mat(1,2)*mat(2,0))*d,
                        (mat(0,0)*mat(2,2)-mat(0,2)*mat(2,0))*d,
                       -(mat(0,0)*mat(1,2)-mat(0,2)*mat(1,0))*d,
                        (mat(1,0)*mat(2,1)-mat(1,1)*mat(2,0))*d,
                       -(mat(0,0)*mat(2,1)-mat(0,1)*mat(2,0))*d,
                        (mat(0,0)*mat(1,1)-mat(0,1)*mat(1,0))*d);
}

} // namespace DDAD
