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
#include "line.h"
#include "predicate.h"

namespace DDAD {

//=============================================================================
// Implementation: Line_2r
//=============================================================================

Line_2r::Line_2r() :
    slope_type_(SLOPE_DEGENERATE) {}

Line_2r::Line_2r(SharedPoint_2r p, SharedPoint_2r q) :
    p_(p),
    q_(q) {
    Update();
}

Line_2r::Line_2r(SlopeType slope_type, const rational& c) :
    slope_type_(slope_type) {
    switch(slope_type_) {
    case SLOPE_PINFINITY:
        p_ = std::make_shared<Point_2r>(c, 0);
        q_ = std::make_shared<Point_2r>(c, 1);
        break;
    case SLOPE_NINFINITY:
        p_ = std::make_shared<Point_2r>(c, 0);
        q_ = std::make_shared<Point_2r>(c, -1);
        break;
    case SLOPE_PZERO:
        p_ = std::make_shared<Point_2r>(0, c);
        q_ = std::make_shared<Point_2r>(1, c);
        break;
    case SLOPE_NZERO:
        p_ = std::make_shared<Point_2r>(0, c);
        q_ = std::make_shared<Point_2r>(-1, c);
        break;
    default:
        slope_type_ = SLOPE_DEGENERATE;
        break;
    }

    if(slope_type_ != SLOPE_DEGENERATE)
        Update();
}

void Line_2r::Update() {
    V_ = (*q_)-(*p_);
    if(*p_ == *q_) {
        slope_type_ = SLOPE_DEGENERATE;
    } else if(p_->x() == q_->x()) {
        if(p_->y() < q_->y()) {
            slope_type_ = SLOPE_PINFINITY;
        } else {
            slope_type_ = SLOPE_NINFINITY;
        }
        N_.set_x(1);
        N_.set_y(0);
        d_ = p_->x();
    } else if(p_->y() == q_->y()) {
        if(p_->x() < q_->x()) {
            slope_type_ = SLOPE_PZERO;
        } else {
            slope_type_ = SLOPE_NZERO;
        }
        N_.set_x(0);
        N_.set_y(1);
        d_ = p_->y();
    } else {
        slope_type_ = SLOPE_GENERAL;
        rational m = (q_->y()-p_->y())/(q_->x()-p_->x());
        d_ = p_->y()-m*p_->x();
        N_.set_x(-m);
        N_.set_y(1);
    }
}

// Predicates =================================================================

namespace Predicate {

Orientation OrientationPQR(const Line_2r& pq, const Point_2r& r) {
    return OrientationPQR(pq.p(), pq.q(), r);
}

Orientation OrientationPQR(const Line_2r& pq, const Vector_2r& r) {
    return OrientationPQR(pq.p(), pq.q(), Point_2r(r.x(), r.y()));
}

bool AIsLeftOfB(const Point_2r& a, const Line_2r& b) {
    return OrientationPQR(b, a) == ORIENTATION_LEFT;
}

bool AIsRightOfB(const Point_2r& a, const Line_2r& b) {
    return OrientationPQR(b, a) == ORIENTATION_RIGHT;
}

bool IsVertical(const Line_2r& l) {
    return (l.slope_type() == SLOPE_PINFINITY ||
            l.slope_type() == SLOPE_NINFINITY);
}

bool IsHorizontal(const Line_2r& l) {
    return (l.slope_type() == SLOPE_PZERO || l.slope_type() == SLOPE_NZERO);
}

bool IsDegenerate(const Line_2r& l) {
    return l.slope_type() == SLOPE_DEGENERATE;
}

bool IsGeneral(const Line_2r& l) {
    return l.slope_type() == SLOPE_GENERAL;
}

bool AreParallel(const Line_2r& a, const Line_2r& b) {
    assert(!IsDegenerate(a) && !IsDegenerate(b));
    return a.N() == b.N();
}

bool AreDisjoint(const Line_2r& a, const Line_2r& b) {
    assert(!IsDegenerate(a) && !IsDegenerate(b));
    return (a.N() == b.N()) && !(a.d() == b.d());
}

bool AreCoincident(const Line_2r& a, const Line_2r& b) {
    assert(!IsDegenerate(a) && !IsDegenerate(b));
    return (a.N() == b.N()) && (a.d() == b.d());
}

} // namespace Predicate

// Accessors/Mutators =========================================================

const Point_2r& Line_2r::p() const {
    return *p_;
}
const Point_2r& Line_2r::q() const {
    return *q_;
}
const Vector_2r& Line_2r::N() const {
    return N_;
}
const rational& Line_2r::d() const {
    return d_;
}
const SlopeType Line_2r::slope_type() const {
    return slope_type_;
}
const Vector_2r& Line_2r::V() const {
    return V_;
}
SharedPoint_2r Line_2r::p_sptr() {
    return p_;
}
SharedPoint_2r Line_2r::q_sptr() {
    return q_;
}

//=============================================================================
// Implementation: Ray_2r
//=============================================================================

Ray_2r::Ray_2r() {}

Ray_2r::Ray_2r(SharedPoint_2r origin, const Vector_2r& direction) :
    origin_(origin),
    direction_(direction),
    support_(origin, std::make_shared<Point_2r>(*origin+direction)) {}

Ray_2r::Ray_2r(SharedPoint_2r origin, SharedPoint_2r to) :
    origin_(origin),
    direction_(*to-*origin),
    support_(origin, to) {}

void Ray_2r::Update() {
    support_.Update();
    direction_ = support_.V();
}

std::string to_string(const Ray_2r& r) {
    std::stringstream ss;
    ss << "origin: " << to_string(r.origin()) << ", direction: " << to_string(r.direction());
    return ss.str();
}

// Predicates =================================================================

namespace Predicate {

Orientation OrientationPQR(const Ray_2r& pq, const Point_2r& r) {
    return OrientationPQR(pq.support().p(), pq.support().q(), r);
}

bool AIsLeftOfB(const Point_2r& a, const Ray_2r& b) {
    return AIsLeftOfB(a, b.support());
}

bool AIsRightOfB(const Point_2r& a, const Ray_2r& b) {
    return AIsRightOfB(a, b.support());
}

bool AreParallel(const Ray_2r& a, const Ray_2r& b) {
    return AreParallel(a.support(), b.support());
}

bool AreParallel(const Line_2r& l, const Ray_2r& r) {
    return AreParallel(l, r.support());
}

bool AreParallel(const Ray_2r& r, const Line_2r& l) {
    return AreParallel(l, r.support());
}

} // namespace Predicate

// Accessors/Mutators =========================================================

const Point_2r& Ray_2r::origin() const {
    return *origin_;
}
const Vector_2r& Ray_2r::direction() const {
    return direction_;
}
const Line_2r& Ray_2r::support() const {
    return support_;
}
Quadrant Ray_2r::quadrant() const {
    return quadrant_;
}
SharedPoint_2r Ray_2r::origin_sptr() {
    return origin_;
}

//=============================================================================
// Segment_2r Implementation
//=============================================================================

Segment_2r::Segment_2r() {}

Segment_2r::Segment_2r(SharedPoint_2r p,
                       SharedPoint_2r q) :
    p_(p),
    q_(q),
    support_(p, q) {}

// Accessors/Mutators =========================================================

const Point_2r& Segment_2r::p() const {
    return *p_;
}
const Point_2r& Segment_2r::q() const {
    return *q_;
}
const Line_2r& Segment_2r::support() const {
    return support_;
}
SharedPoint_2r Segment_2r::p_sptr() {
    return p_;
}
SharedPoint_2r Segment_2r::q_sptr() {
    return q_;
}
void Segment_2r::set_p(SharedPoint_2r p) {
    p_ = p;
}
void Segment_2r::set_q(SharedPoint_2r q) {
    q_ = q;
}

//=============================================================================
// Line_3r Implementation
//=============================================================================

Line_3r::Line_3r() {}

Line_3r::Line_3r(SharedPoint_3r p, SharedPoint_3r q) :
    p_(p),
    q_(q) {
    Update();
}

void Line_3r::Update() {
    V_ = (*q_)-(*p_);
    /*
    if(*p_ == *q_) {
        slope_type_ = SLOPE_DEGENERATE;
    } else if(p_->x() == q_->x()) {
        if(p_->y() < q_->y()) {
            slope_type_ = SLOPE_PINFINITY;
        } else {
            slope_type_ = SLOPE_NINFINITY;
        }
        N_.set_x(1);
        N_.set_y(0);
        d_ = p_->x();
    } else if(p_->y() == q_->y()) {
        if(p_->x() < q_->x()) {
            slope_type_ = SLOPE_PZERO;
        } else {
            slope_type_ = SLOPE_NZERO;
        }
        N_.set_x(0);
        N_.set_y(1);
        d_ = p_->y();
    } else {
        slope_type_ = SLOPE_GENERAL;
        rational m = (q_->y()-p_->y())/(q_->x()-p_->x());
        d_ = p_->y()-m*p_->x();
        N_.set_x(-m);
        N_.set_y(1);
    }
    */
}

// Accessors/Mutators =========================================================

const Point_3r& Line_3r::p() const {
    return *p_;
}
const Point_3r& Line_3r::q() const {
    return *q_;
}
const Vector_3r& Line_3r::N() const {
    return N_;
}
const rational& Line_3r::d() const {
    return d_;
}
const Vector_3r& Line_3r::V() const {
    return V_;
}
SharedPoint_3r Line_3r::p_sptr() {
    return p_;
}
SharedPoint_3r Line_3r::q_sptr() {
    return q_;
}

//=============================================================================
// Implementation: Ray_3r
//=============================================================================

Ray_3r::Ray_3r() {}

Ray_3r::Ray_3r(SharedPoint_3r origin, const Vector_3r& direction) :
    origin_(origin),
    direction_(direction),
    support_(origin, std::make_shared<Point_3r>(*origin+direction)) {}

Ray_3r::Ray_3r(SharedPoint_3r origin, SharedPoint_3r to) :
    origin_(origin),
    direction_(*to-*origin),
    support_(origin, to) {}

void Ray_3r::Update() {
    support_.Update();
    direction_ = support_.V();
}

std::string to_string(const Ray_3r& r) {
    std::stringstream ss;
    ss << "origin: " << to_string(r.origin()) << ", direction: " << to_string(r.direction());
    return ss.str();
}

// Accessors/Mutators =========================================================

const Point_3r& Ray_3r::origin() const {
    return *origin_;
}
const Vector_3r& Ray_3r::direction() const {
    return direction_;
}
const Line_3r& Ray_3r::support() const {
    return support_;
}
SharedPoint_3r Ray_3r::origin_sptr() {
    return origin_;
}

//=============================================================================
// Segment_3r Implementation
//=============================================================================

Segment_3r::Segment_3r() {}

Segment_3r::Segment_3r(SharedPoint_3r p,
                       SharedPoint_3r q) :
    p_(p),
    q_(q),
    support_(p, q) {}

// Accessors/Mutators =========================================================

const Point_3r& Segment_3r::p() const {
    return *p_;
}
const Point_3r& Segment_3r::q() const {
    return *q_;
}
const Line_3r& Segment_3r::support() const {
    return support_;
}
SharedPoint_3r Segment_3r::p_sptr() {
    return p_;
}
SharedPoint_3r Segment_3r::q_sptr() {
    return q_;
}
void Segment_3r::set_p(SharedPoint_3r p) {
    p_ = p;
}
void Segment_3r::set_q(SharedPoint_3r q) {
    q_ = q;
}

} // namespace DDAD
