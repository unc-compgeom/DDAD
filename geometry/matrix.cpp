/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>,
 *          Steven Love <slove13@cs.unc.edu>}
 * @date 10/29/2013
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
