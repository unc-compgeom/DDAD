/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 07/24/2013
 */

#include "common.h"
#include "point.h"
#include "matrix.h"

namespace DDAD {

namespace Predicate {

//! \degree 2
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

//! \degree 2
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

//! \degree 2
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

} // namespace Predicate

} // namespace DDAD


