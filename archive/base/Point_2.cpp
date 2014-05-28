/**
 * Filename: Point_2.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 08 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "Point_2.h"

std::string Point_2::Std_point::to_string() const {
    std::ostringstream os;
    if (id() >= 0) { os << id() << ": "; }
    os << "(" << x() << ", " << y() << ")";
    return os.str();
}

Point_2::Orientation Point_2::orientation( const Point_2::Std_point& p,
        const Point_2::Std_point& q, const Point_2::Std_point& r) {

    typedef long Type;
    DDAD::Sign orient_sign = DDAD::sign( Algebraic_predicates::det2x2(
                (Type)q.x()-p.x(), (Type)q.y()-p.y(),
                (Type)r.x()-p.x(), (Type)r.y()-p.y()));
    switch (orient_sign) {
        case DDAD::POSITIVE: return ORIENT_LEFT;
        case DDAD::NEGATIVE: return ORIENT_RIGHT;
        case DDAD::ZERO: return ORIENT_COLINE;
        default: rTerminate("Case not coverted", DDAD::Case_not_covered_error());
    }
}

Point_2::Closer Point_2::closer(const Point_2::Std_point& a,
        const Point_2::Std_point& b, const Point_2::Std_point& q) {
    double adx = q.x()-a.x();
    double ady = q.y()-a.y();
    double bdx = q.x()-b.x();
    double bdy = q.y()-b.y();
    DDAD::Sign sq_dist_sign = DDAD::sign(adx*adx + ady*ady - bdx*bdx - bdy*bdy);
    switch (sq_dist_sign) {
        case DDAD::POSITIVE: return CLOSER_TO_SECOND;
        case DDAD::NEGATIVE: return CLOSER_TO_FIRST;
        case DDAD::ZERO: return CLOSER_EQUIDISTANT;
        default: rTerminate("Case not covered", DDAD::Case_not_covered_error());
    }
}

bool Point_2::on_vertical_line(const Point_2::Std_point& p,
        const Point_2::Std_point& q) {
    return p.x() == q.x();
}

bool Point_2::precedes(const Point_2::Std_point& p,
        const Point_2::Std_point& q) {
    return p.x() < q.x() || (p.x() == q.x() && p.y() < q.y());
}



