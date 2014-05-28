/**
 * Filename: Predicates_2.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 08 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "Predicates_2.h"

bool Predicates_2::in_closed_interval(int low, int high, int q) {
    rAssert(low <= high);
    return low <= q && q <= high;
}


Predicates_2::Seg_inter Predicates_2::intersect_interior(
        const Segment_2::Std_segment& s1, const Segment_2::Std_segment& s2) {

    // first get 4 orientations
    Point_2::Orientation orient_s1_s2p1 =
        Point_2::orientation(s1.p1(), s1.p2(), s2.p1());
    Point_2::Orientation orient_s1_s2p2 =
        Point_2::orientation(s1.p1(), s1.p2(), s2.p2());
    Point_2::Orientation orient_s2_s1p1 =
        Point_2::orientation(s2.p1(), s2.p2(), s1.p1());
    Point_2::Orientation orient_s2_s1p2 =
        Point_2::orientation(s2.p1(), s2.p2(), s1.p2());

    bool all_colinear = orient_s1_s2p1 == Point_2::ORIENT_COLINE
            && orient_s1_s2p1 == orient_s1_s2p2
            && orient_s1_s2p1 == orient_s2_s1p1
            && orient_s1_s2p1 == orient_s2_s1p2;

    bool any_colinear = orient_s1_s2p1 == Point_2::ORIENT_COLINE
            || orient_s1_s2p2 == Point_2::ORIENT_COLINE
            || orient_s2_s1p1 == Point_2::ORIENT_COLINE
            || orient_s2_s1p2 == Point_2::ORIENT_COLINE;

    // if all the points are colinear then we have either a open segment
    // intersection or no intersection
    // if they are not all colinear then we either have a point intersection or
    // no intersection.
    if (all_colinear && (Predicates_2::on_open_segment(s1,s2.p1())
                || Predicates_2::on_open_segment(s1,s2.p2())
                || Predicates_2::on_open_segment(s2,s1.p1())
                || Predicates_2::on_open_segment(s2,s1.p2()))) {
        return SEG_INTER_OPEN_SEG;
    } else if (!any_colinear && orient_s1_s2p1 != orient_s1_s2p2
            && orient_s2_s1p1 != orient_s2_s1p2) {
        return SEG_INTER_POINT;
    } else {
        return SEG_INTER_EMPTY;
    }
}


bool Predicates_2::above(const Segment_2::Std_segment& s,
        const Point_2::Std_point& p) {

    // get the orientation of the points
    Point_2::Orientation orient = Point_2::orientation(s.p1(), s.p2(), p);

    // verify preconditions
    rAssert(!Point_2::on_vertical_line(s.p1(), s.p2()));

    return orient == (Point_2::precedes(s.p1(), s.p2()) ? Point_2::ORIENT_LEFT : Point_2::ORIENT_RIGHT);
}

bool Predicates_2::below(const Segment_2::Std_segment& s,
        const Point_2::Std_point& p) {

    // get the orientation of the points
    Point_2::Orientation orient = Point_2::orientation(s.p1(), s.p2(), p);

    // verify preconditions
    rAssert(!Point_2::on_vertical_line(s.p1(), s.p2()));

    return orient == (Point_2::precedes(s.p1(), s.p2()) ? Point_2::ORIENT_RIGHT : Point_2::ORIENT_LEFT);
}



bool Predicates_2::on_open_segment(const Segment_2::Std_segment& s,
        const Point_2::Std_point& p) {

    // p is on the interrior of s if it lays on the line though s and the vectors
    // s.p1()-p and s.p2()-p are in oppisite directions.  Note that a zero
    // evaluation of the dot product means p == s.p1() or p == s.p2().
    return Point_2::orientation(s.p1(), s.p2(), p) == Point_2::ORIENT_COLINE
        && Algebraic_predicates::dot(s.p1().x()- p.x(), s.p1().y()- p.y(),
                s.p2().x()- p.x(), s.p2().y()- p.y()) == DDAD::NEGATIVE;
}


bool Predicates_2::on_closed_segment(const Segment_2::Std_segment& s,
        const Point_2::Std_point& p) {

    // p is on the interrior of s if it lays on the line though s and the vectors
    // s.p1()-p and s.p2()-p are in oppisite directions.  Note that a zero
    // evaluation of the dot product means p == s.p1() or p == s.p2().
    return Point_2::orientation(s.p1(), s.p2(), p) == Point_2::ORIENT_COLINE
        && Algebraic_predicates::dot(s.p1().x()- p.x(), s.p1().y()- p.y(),
                s.p2().x()- p.x(), s.p2().y()- p.y()) != DDAD::POSITIVE;
}



