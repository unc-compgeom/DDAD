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

#ifndef GE_PREDICATE_H
#define GE_PREDICATE_H

#include "common.h"
#include "point.h"
#include "matrix.h"

namespace DDAD {

namespace Predicate {

bool AIsLeftOfB(const Point_2i& a, const Point_2i& b);
bool AIsLeftOfB(const Point_2f& a, const Point_2f& b);
bool AIsLeftOfB(const Point_2r& a, const Point_2r& b);

bool AIsBelowB(const Point_2i& a, const Point_2i& b);
bool AIsBelowB(const Point_2f& a, const Point_2f& b);
bool AIsBelowB(const Point_2r& a, const Point_2r& b);

Orientation OrientationPQR(const Point_2i& p, const Point_2i& q,
                           const Point_2i& r);
Orientation OrientationPQR(const Point_2f& p, const Point_2f& q,
                           const Point_2f& r);
Orientation OrientationPQR(const Point_2r& p, const Point_2r& q,
                           const Point_2r& r);

bool RIsLeftOrInsidePQ(const Point_2r& p, const Point_2r& q, const Point_2r& r);

inline bool AIsLeftOfB(const Point_2i& a, const Point_2i& b) {
    return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
}

inline bool AIsLeftOfB(const Point_2f& a, const Point_2f& b) {
    return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
}

inline bool AIsLeftOfB(const Point_2r& a, const Point_2r& b) {
    return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
}

inline bool AIsBelowB(const Point_2i& a, const Point_2i& b) {
    return a.y() < b.y() || (a.y() == b.y() && a.x() < b.x());
}

inline bool AIsBelowB(const Point_2f& a, const Point_2f& b) {
    return a.y() < b.y() || (a.y() == b.y() && a.x() < b.x());
}

inline bool AIsBelowB(const Point_2r& a, const Point_2r& b) {
    return a.y() < b.y() || (a.y() == b.y() && a.x() < b.x());
}

inline rational InCircle(const Point_3r& a, const Point_3r& b,
                         const Point_3r& c, const Point_3r& d) {
    auto m00 = a.x()-d.x();
    auto m01 = a.y()-d.y();
    auto m02 = m00*m00+m01*m01;
    auto m10 = b.x()-d.x();
    auto m11 = b.y()-d.y();
    auto m12 = m10*m10+m11*m11;
    auto m20 = c.x()-d.x();
    auto m21 = c.y()-d.y();
    auto m22 = m20*m20+m21*m21;
    return Determinant(Matrix_3x3r(
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22
    ));
}

inline rational Orient2D(const Point_3r& a, const Point_3r& b,
                         const Point_3r& c) {
    return Determinant(Matrix_2x2r(
        a.x()-c.x(), a.y()-c.y(),
        b.x()-c.x(), b.y()-c.y()
    ));
}

/*
inline Orientation OrientationPQR(const Point_3r& p, const Point_3r& q,
                                  const Point_3r& r) {

}
*/

} // namespace Predicate

} // namespace DDAD

#endif // GE_PREDICATE_H
