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
 * @note Code modeled after Eberly's intersection classes (geometrictools.com)
 */

#ifndef GE_INTERSECTION_H
#define GE_INTERSECTION_H

#include "common.h"
#include "vector.h"
#include "visual.h"
#include "polygon.h"
#include "sphere.h"
#include "pointset.h"
#include "polytope.h"
#include "quadedge.h"

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

//=============================================================================
// Interface: Ray_3rPolyline_2r
//=============================================================================

namespace Intersection {

class Ray_3rSphere_3r {
public:
    enum class IntersectionType {
        EMPTY,
        POINT,
        SEGMENT
    };

    Ray_3rSphere_3r() :
        ray_(nullptr),
        sphere_(nullptr),
        type_(IntersectionType::EMPTY),
        time_enter_(0),
        time_exit_(0) {}

    Ray_3rSphere_3r(const Ray_3r *ray, const Sphere_3r *sphere) :
        ray_(ray),
        sphere_(sphere) {

        Vector_3r m = sphere->center() - ray->origin();
        rational a = Dot(ray->direction(), ray->direction());
        rational b = -2 * Dot(m, ray->direction());
        rational c = Dot(m, m) - sphere->radius() * sphere->radius();
        rational discr = b * b - 4 * a * c;

        // no real roots: ray completely missed the sphere
        if (discr < 0) {
            type_ = IntersectionType::EMPTY;
            time_enter_ = 0; // should be infinity
            time_exit_ = 0;
            return;
        }

        // double root: ray grazes sphere tangentially
        if (discr == 0) {
            type_ = IntersectionType::POINT;
            time_enter_ = -b / (2 * a);
            time_exit_ = time_enter_;
            return;
        }

        // two real roots: ray intersects normally, smaller root occurs first
        type_ = IntersectionType::SEGMENT;
        time_enter_ = (-b - sqrt(discr.get_d())) / (2 * a);
        time_exit_ = (-b + sqrt(discr.get_d())) / (2 * a);
    }

    const Ray_3r* ray() const {
        return ray_;
    }
    const Sphere_3r* sphere() const {
        return sphere_;
    }
    IntersectionType type() const {
        return type_;
    }
    const rational& time_enter() const {
        return time_enter_;
    }
    const rational& time_exit() const {
        return time_exit_;
    }

private:
    const Ray_3r *ray_;
    const Sphere_3r *sphere_;
    IntersectionType type_;
    rational time_enter_;
    rational time_exit_;
};

namespace Toleranced {

/*!
 * @brief Represents the intersection of a 3D ray and a 2D polyline.
 */
class Ray_3rPolyline_2r {
public:
    enum Type {
        INTERSECTION_EMPTY,
        INTERSECTION_POINT,
        INTERSECTION_COINCIDENT
    };

    Ray_3rPolyline_2r() :
        ray_(nullptr),
        polyline_(nullptr),
        type_(Type::INTERSECTION_EMPTY) {}

    Ray_3rPolyline_2r(const Ray_3r* ray, const Polyline_2r* polyline) :
        ray_(ray),
        polyline_(polyline) {

        type_ = Type::INTERSECTION_EMPTY;

        rational earliest_time = 99999999; // FIXME
        for (auto vertex : polyline_->vertices()) {
            Sphere_3r tol_vertex(*vertex, 8.0);
            Ray_3rSphere_3r isect(ray, &tol_vertex);
            if (isect.type() != Ray_3rSphere_3r::IntersectionType::EMPTY) {
                type_ = Type::INTERSECTION_POINT; // FIXME
                if (isect.time_enter() < earliest_time) {
                    earliest_time = isect.time_enter();
                }
            }
        }

        if (type_ != Type::INTERSECTION_EMPTY) {
            time_ = earliest_time;
        }

    }

    const Type type() const {
        return type_;
    }
    const rational& time() const {
        return time_;
    }

protected:
    const Ray_3r* ray_;
    const Polyline_2r* polyline_;
    Type type_;
    rational time_;
};

class Ray_3rPointSet_3r {
public:
    enum Type {
        INTERSECTION_EMPTY,
        INTERSECTION_POINT
    };

    Ray_3rPointSet_3r() :
        ray_(nullptr),
        pointset_(nullptr),
        type_(Type::INTERSECTION_EMPTY) {}

    Ray_3rPointSet_3r(const Ray_3r *ray, const PointSet_3r *pointset) :
        ray_(ray),
        pointset_(pointset) {

        type_ = Type::INTERSECTION_EMPTY;

        rational earliest_time = 99999999; // FIXME
        for (auto point : pointset_->points()) {
            Sphere_3r tol_point(*point, 8.0);
            Ray_3rSphere_3r isect(ray, &tol_point);
            if (isect.type() != Ray_3rSphere_3r::IntersectionType::EMPTY) {
                type_ = Type::INTERSECTION_POINT; // FIXME
                if (isect.time_enter() < earliest_time) {
                    earliest_time = isect.time_enter();
                }
            }
        }

        if (type_ != Type::INTERSECTION_EMPTY) {
            time_ = earliest_time;
        }

    }

    const Type type() const {
        return type_;
    }
    const rational& time() const {
        return time_;
    }


protected:
    const Ray_3r *ray_;
    const PointSet_3r *pointset_;
    Type type_;
    rational time_;
};

class Ray_3rPolytope_3r {
public:
    enum Type {
        INTERSECTION_EMPTY,
        INTERSECTION_POINT
    };

    Ray_3rPolytope_3r() :
        ray_(nullptr),
        polytope_(nullptr),
        type_(Type::INTERSECTION_EMPTY) {}

    Ray_3rPolytope_3r(const Ray_3r *ray, const Polytope_3r *polytope) :
        ray_(ray),
        polytope_(polytope) {

        type_ = Type::INTERSECTION_EMPTY;

        rational earliest_time = 99999999; // FIXME
        QuadEdge::CellVertexIterator cellVerts(polytope_->cell());
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            Sphere_3r tol_point(*v->pos, 8.0);
            Ray_3rSphere_3r isect(ray, &tol_point);
            if (isect.type() != Ray_3rSphere_3r::IntersectionType::EMPTY) {
                type_ = Type::INTERSECTION_POINT; // FIXME
                if (isect.time_enter() < earliest_time) {
                    earliest_time = isect.time_enter();
                }
            }
        }

        if (type_ != Type::INTERSECTION_EMPTY) {
            time_ = earliest_time;
        }

    }

    const Type type() const {
        return type_;
    }
    const rational& time() const {
        return time_;
    }


protected:
    const Ray_3r *ray_;
    const Polytope_3r *polytope_;
    Type type_;
    rational time_;
};

}

}



} // namespace DDAD

#endif // GE_INTERSECTION_H
