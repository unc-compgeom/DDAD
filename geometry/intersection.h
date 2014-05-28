/*
 * This file is part of RationalCAD.
 *
 * RationalCAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * RationalCAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with RationalCAD. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-07-11
 * @note Code modeled after Eberly's intersection classes (geometrictools.com)
 */

#ifndef GE_INTERSECTION_H
#define GE_INTERSECTION_H

#include "common.h"
#include "vector.h"
#include "visual.h"

namespace DDAD {

class Line_2r;
class Ray_2r;
class Segment_2r;

/*
namespace Intersection {

enum Type {
    INTERSECTION_EMPTY,
    INTERSECTION_POINT,
    INTERSECTION_COINCIDENT
};

}
*/

//=============================================================================
// Interface: Line_2rLine_2r
//=============================================================================

namespace Intersection {

/*!
 * @brief Represents the intersection of two 2D lines.
 *
 * Maintains raw pointers to the two lines, and is not responsible for deleting
 * these lines. This class does not modify the lines. Users of this class are
 * responsible for ensuring that the pointers remain valid during an instance's
 * lifetime.
 *
 * The intersection, like most points, is canonically allocated on the heap.
 */
class Line_2rLine_2r {
public:
    enum Type {
        INTERSECTION_EMPTY,
        INTERSECTION_POINT,
        INTERSECTION_COINCIDENT
    };

    Line_2rLine_2r();
    Line_2rLine_2r(const Line_2r* line_imp, const Line_2r* line_par);

    void Update();

    const Point_2r& intersection() const;
    const Type type() const;
    const rational& time() const;
    SharedPoint_2r intersection_sptr();

private:
    const Line_2r* line_imp_;
    const Line_2r* line_par_;

    SharedPoint_2r intersection_;
    Type type_;
    rational time_;
};

}

namespace Predicate {
    bool IsEmpty(const Intersection::Line_2rLine_2r& i);
}

//=============================================================================
// Interface: Line_2_Ray_2
//=============================================================================

namespace Intersection {

/*!
 * @brief Represents the intersection of a 2D line and a 2D ray.
 */
class Line_2rRay_2r {
public:
    enum Type {
        INTERSECTION_EMPTY,
        INTERSECTION_POINT,
        INTERSECTION_COINCIDENT
    };

    Line_2rRay_2r();
    Line_2rRay_2r(const Line_2r* line, const Ray_2r* ray);

    const Point_2r& intersection() const;
    const Type type() const;
    const rational& time() const;
    SharedPoint_2r intersection_sptr();

protected:
    const Line_2r* line_;
    const Ray_2r* ray_;

    SharedPoint_2r intersection_;
    Type type_;
    rational time_;
};

}

namespace Construction {
Point_2r LastBefore(const Line_2r& l, const Ray_2r& r);
Point_2r LastAfter(const Line_2r& l, const Ray_2r& r);
}

namespace Predicate {
bool IsEmpty(const Intersection::Line_2rRay_2r& i);
}

//=============================================================================
// Interface: Line_2rSegment_2r
//=============================================================================

namespace Intersection {

class Line_2rSegment_2r {
public:
    enum Type {
        INTERSECTION_EMPTY,
        INTERSECTION_POINT,
        INTERSECTION_COINCIDENT
    };

    Line_2rSegment_2r();
    Line_2rSegment_2r(const Segment_2r* segment, const Line_2r* line);

    const Type type() const { return type_; }
    const rational& time() const { return time_; }
    SharedPoint_2r intersection_sptr() { return intersection_; }

protected:
    const Segment_2r* segment_;
    const Line_2r* line_;

    SharedPoint_2r intersection_;
    Type type_;
    rational time_;
};

}

//=============================================================================
// Interface: Ray_2rSegment_2r
//=============================================================================

namespace Intersection {

/*!
 * @brief Represents the intersection of a 2D ray and a 2D segment.
 */
class Ray_2rSegment_2r {
public:
    enum Type {
        INTERSECTION_EMPTY,
        INTERSECTION_POINT,
        INTERSECTION_COINCIDENT
    };

    Ray_2rSegment_2r();
    Ray_2rSegment_2r(const Ray_2r* ray, const Segment_2r* segment);

    const Point_2r& intersection() const;
    const Type type() const;
    const rational& time() const;
    SharedPoint_2r intersection_sptr();

protected:
    const Ray_2r* ray_;
    const Segment_2r* segment_;

    SharedPoint_2r intersection_;
    Type type_;
    rational time_;
};

}

namespace Predicate {
bool IsEmpty(const Intersection::Ray_2rSegment_2r& i);
}


} // namespace DDAD

#endif // GE_INTERSECTION_H
