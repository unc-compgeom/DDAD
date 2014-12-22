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

#ifndef GE_LINE_H
#define GE_LINE_H

#include "common.h"
#include "point.h"
#include "vector.h"

namespace DDAD {

//=============================================================================
// Line_2 Interface
//=============================================================================

enum SlopeType {
    SLOPE_GENERAL,
    SLOPE_PINFINITY,
    SLOPE_NINFINITY,
    SLOPE_PZERO,
    SLOPE_NZERO,
    SLOPE_DEGENERATE
};

/*!
 * @brief The Line_2r class represents an oriented rational line in the plane.
 *
 * An oriented line \f$ \ell \f$ in the plane can be represented in several
 * different ways. This class maintains three different representations:
 * implicit, parametric, and an ordered pair of points \f$ p \f$ and
 * \f$ q \f$.
 *
 * ### Implicit
 *
 * The implicit (general) form of a line equation is written \f$ ax+by=c \f$.
 * This form extends readily to three dimensions, where it gives the familiar
 * equation of a plane. Like its three dimensional counterpart, the equation
 * can be viewed as the dot product of \f$ \ell \f$'s normal \f$
 * \vec{N}=\langle a, b \rangle \f$ with a variable point \f$ X=(x, y) \f$ to
 * give the distance \f$d=c\f$ of \f$ X \f$ along \f$ \vec{N} \f$.
 *
 * ### Parametric
 *
 * The parametric form of a line is written \f$X = p+k\vec{V}\f$.
 */
class Line_2r {
public:
    Line_2r();
    Line_2r(SharedPoint_2r p, SharedPoint_2r q);
    Line_2r(SlopeType slope_type, const rational& d);

    void Update();

    const Point_2r& p() const;
    const Point_2r& q() const;
    const SlopeType slope_type() const;
    const Vector_2r& N() const;
    const rational& d() const;
    const Vector_2r& V() const;

    SharedPoint_2r p_sptr();
    SharedPoint_2r q_sptr();

private:
    SharedPoint_2r p_;
    SharedPoint_2r q_;

    SlopeType slope_type_;

    // implicit representation, N.X = d
    Vector_2r N_;
    rational d_;

    // parametric representation, X = p+kV
    Vector_2r V_;
};

namespace Predicate {
Orientation OrientationPQR(const Line_2r& pq, const Point_2r& r);
Orientation OrientationPQR(const Line_2r& pq, const Vector_2r& r);
bool AIsLeftOfB(const Point_2r& a, const Line_2r& b);
bool AIsRightOfB(const Point_2r& a, const Line_2r& b);
bool IsVertical(const Line_2r& l);
bool IsHorizontal(const Line_2r& l);
bool IsDegenerate(const Line_2r& l);
bool IsGeneral(const Line_2r& l);
bool AreParallel(const Line_2r& a, const Line_2r& b);
bool AreDisjoint(const Line_2r& a, const Line_2r& b);
bool AreCoincident(const Line_2r& a, const Line_2r& b);
}

//=============================================================================
// Interface: Ray_2r
//=============================================================================

class Ray_2r {
public:
    Ray_2r();
    Ray_2r(SharedPoint_2r origin, const Vector_2r& direction);
    Ray_2r(SharedPoint_2r origin, SharedPoint_2r to);

    void Update();

    const Point_2r& origin() const;
    const Vector_2r& direction() const;
    const Line_2r& support() const;
    Quadrant quadrant() const;

    SharedPoint_2r origin_sptr();

private:
    SharedPoint_2r origin_;
    Vector_2r direction_;
    Line_2r support_;
    Quadrant quadrant_;
};

std::string to_string(const Ray_2r& r);

namespace Predicate {
Orientation OrientationPQR(const Ray_2r& pq, const Point_2r& r);
bool AIsLeftOfB(const Point_2r& a, const Ray_2r& b);
bool AIsRightOfB(const Point_2r& a, const Ray_2r& b);
bool AreParallel(const Ray_2r& a, const Ray_2r& b);
bool AreParallel(const Line_2r& l, const Ray_2r& r);
bool AreParallel(const Ray_2r& r, const Line_2r& l);
}

//=============================================================================
// Interface: Segment_2r
//=============================================================================

class Segment_2r {
public:
    Segment_2r();
    Segment_2r(SharedPoint_2r p, SharedPoint_2r q);

    const Point_2r& p() const;
    const Point_2r& q() const;
    const Line_2r& support() const;

    SharedPoint_2r p_sptr();
    SharedPoint_2r q_sptr();

    void set_p(SharedPoint_2r p);
    void set_q(SharedPoint_2r q);

private:
    SharedPoint_2r p_;
    SharedPoint_2r q_;
    Line_2r support_;
};

//=============================================================================
// Line_3 Interface
//=============================================================================

class Line_3r {
public:
    Line_3r();
    Line_3r(SharedPoint_3r p, SharedPoint_3r q);

    void Update();

    const Point_3r& p() const;
    const Point_3r& q() const;
    const SlopeType slope_type() const;
    const Vector_3r& N() const;
    const rational& d() const;
    const Vector_3r& V() const;

    SharedPoint_3r p_sptr();
    SharedPoint_3r q_sptr();

private:
    SharedPoint_3r p_;
    SharedPoint_3r q_;

    // implicit representation, N.X = d
    Vector_3r N_;
    rational d_;

    // parametric representation, X = p+kV
    Vector_3r V_;
};

namespace Predicate {
bool IsDegenerate(const Line_3r& l);
}

//=============================================================================
// Interface: Ray_3r
//=============================================================================

class Ray_3r {
public:
    Ray_3r();
    Ray_3r(SharedPoint_3r origin, const Vector_3r& direction);
    Ray_3r(SharedPoint_3r origin, SharedPoint_3r to);

    void Update();

    const Point_3r& origin() const;
    const Vector_3r& direction() const;
    const Line_3r& support() const;

    SharedPoint_3r origin_sptr();

private:
    SharedPoint_3r origin_;
    Vector_3r direction_;
    Line_3r support_;
};

std::string to_string(const Ray_3r& r);

//=============================================================================
// Interface: Segment_3r
//=============================================================================

class Segment_3r {
public:
    Segment_3r();
    Segment_3r(SharedPoint_3r p, SharedPoint_3r q);

    const Point_3r& p() const;
    const Point_3r& q() const;
    const Line_3r& support() const;

    SharedPoint_3r p_sptr();
    SharedPoint_3r q_sptr();

    void set_p(SharedPoint_3r p);
    void set_q(SharedPoint_3r q);

private:
    SharedPoint_3r p_;
    SharedPoint_3r q_;
    Line_3r support_;
};

} // namespace DDAD

#endif // GE_LINE_H
