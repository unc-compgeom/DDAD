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
#include "intersection.h"
#include "arithmetic.h"
#include "line.h"

namespace DDAD {

//=============================================================================
// Implementation: Line_2rLine_2r
//=============================================================================

namespace Intersection {

Line_2rLine_2r::Line_2rLine_2r() :
    type_(INTERSECTION_EMPTY),
    line_imp_(nullptr),
    line_par_(nullptr) {}

Line_2rLine_2r::Line_2rLine_2r(const Line_2r* line_imp,
                               const Line_2r* line_par) :
    line_imp_(line_imp),
    line_par_(line_par) {
    assert(!Predicate::IsDegenerate(*line_imp_) &&
           !Predicate::IsDegenerate(*line_par_));

    if (Predicate::AreDisjoint(*line_imp_, *line_par_)) {
        type_ = INTERSECTION_EMPTY;
        return;
    } else if (Predicate::AreCoincident(*line_imp_, *line_par_)) {
        type_ = INTERSECTION_COINCIDENT;
        return;
    } else {
        type_ = INTERSECTION_POINT;
        Update();
        return;
    }
}

//! \todo div by zero?
void Line_2rLine_2r::Update() {
    time_ = (line_imp_->d()-Dot(line_imp_->N(), line_par_->p()))/
            (Dot(line_imp_->N(), line_par_->V()));
    if (!intersection_) {
        intersection_ = std::make_shared<Point_2r>(line_par_->p()+
                                                   line_par_->V()*time_);
    } else {
        *intersection_ = line_par_->p()+line_par_->V()*time_;
    }
}

// Accessors/Mutators =========================================================

const Point_2r& Line_2rLine_2r::intersection() const {
    assert(type_ == INTERSECTION_POINT);
    return *intersection_;
}
const Line_2rLine_2r::Type Line_2rLine_2r::type() const {
    return type_;
}
const rational& Line_2rLine_2r::time() const {
    assert(type_ == INTERSECTION_POINT);
    return time_;
}
SharedPoint_2r Line_2rLine_2r::intersection_sptr() {
    assert(type_ == INTERSECTION_POINT);
    return intersection_;
}

} // namespace Intersection

// Predicates =================================================================

namespace Predicate {

bool IsEmpty(const Intersection::Line_2rLine_2r& i) {
    return i.type() == Intersection::Line_2rLine_2r::INTERSECTION_EMPTY;
}

} // namespace Predicate

//=============================================================================
// Implementation: Line_2rRay_2r
//=============================================================================

namespace Intersection {

Line_2rRay_2r::Line_2rRay_2r() :
    type_(INTERSECTION_EMPTY),
    line_(nullptr),
    ray_(nullptr) {}

Line_2rRay_2r::Line_2rRay_2r(const Line_2r* line, const Ray_2r* ray) :
    line_(line),
    ray_(ray) {
    Line_2rLine_2r isect_support(line, &ray->support());
    switch (isect_support.type()) {
    case INTERSECTION_EMPTY:
        type_ = INTERSECTION_EMPTY;
        break;
    case INTERSECTION_COINCIDENT:
        type_ = INTERSECTION_COINCIDENT;
        break;
    case INTERSECTION_POINT:
        if(isect_support.time() < 0) {
            type_ = INTERSECTION_EMPTY;
        } else {
            type_ = INTERSECTION_POINT;
            intersection_ = isect_support.intersection_sptr();
            time_ = isect_support.time();
        }
        break;
    }
}

// Accessors/Mutators =========================================================

const Point_2r& Line_2rRay_2r::intersection() const {
    return *intersection_;
}
const Line_2rRay_2r::Type Line_2rRay_2r::type() const {
    return type_;
}
const rational& Line_2rRay_2r::time() const {
    return time_;
}
SharedPoint_2r Line_2rRay_2r::intersection_sptr() {
    return intersection_;
}

} // namespace Intersection

// Constructions ==============================================================

namespace Construction {

Point_2r LastAfter(const Line_2r& l, const Ray_2r& r) {
    Intersection::Line_2rRay_2r isect(&l, &r);
    return r.origin()+r.direction()*Ceil(isect.time());
}

Point_2r LastBefore(const Line_2r& l, const Ray_2r& r) {
    Intersection::Line_2rRay_2r isect(&l, &r);
    return r.origin()+r.direction()*Floor(isect.time());
}

} // namespace Construction

// Predicates =================================================================

namespace Predicate {

bool IsEmpty(const Intersection::Line_2rRay_2r& i) {
    return i.type() == Intersection::Line_2rRay_2r::INTERSECTION_EMPTY;
}

} // namespace Predicate

//=============================================================================
// Implementation: Line_2rSegment_2r
//=============================================================================

//=============================================================================
// Implementation: Ray_2rSegment_2r
//=============================================================================

namespace Intersection {

Ray_2rSegment_2r::Ray_2rSegment_2r() :
    type_(INTERSECTION_EMPTY),
    ray_(nullptr),
    segment_(nullptr) {}

Ray_2rSegment_2r::Ray_2rSegment_2r(const Ray_2r *ray,
                                   const Segment_2r *segment) :
    ray_(ray),
    segment_(segment) {
    Line_2rLine_2r isect_support(&segment->support(), &ray->support());
    switch(isect_support.type()) {
    case INTERSECTION_EMPTY:
        type_ = INTERSECTION_EMPTY;
        break;
    case INTERSECTION_COINCIDENT:
        type_ = INTERSECTION_COINCIDENT;
        time_ = 0;
        break;
    case INTERSECTION_POINT:
        if(isect_support.time() < 0) {
            type_ = INTERSECTION_EMPTY;
        } else {
            auto pt = Dot(segment->support().V(), segment->p());
            auto qt = Dot(segment->support().V(), segment->q());
            auto it = Dot(segment->support().V(), isect_support.intersection());
            if(it <= qt && it >= pt) {
                type_ = INTERSECTION_POINT;
                intersection_ = isect_support.intersection_sptr();
                time_ = isect_support.time();
            } else {
                type_ = INTERSECTION_EMPTY;
            }
        }
        break;
    }
}

// Accessors/Mutators =========================================================

const Point_2r& Ray_2rSegment_2r::intersection() const {
    return *intersection_;
}
const Ray_2rSegment_2r::Type Ray_2rSegment_2r::type() const {
    return type_;
}
const rational& Ray_2rSegment_2r::time() const {
    return time_;
}
SharedPoint_2r Ray_2rSegment_2r::intersection_sptr() {
    return intersection_;
}

} // namespace Intersection

// Predicates =================================================================

namespace Predicate {

bool IsEmpty(const Intersection::Ray_2rSegment_2r& i) {
    return i.type() == Intersection::Ray_2rSegment_2r::INTERSECTION_EMPTY;
}

} // namespace Predicate

} // namespace DDAD

