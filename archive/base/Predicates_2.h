/**
 * Filename: Predicates_2.h
 * Project name: degDrivenAlgoDesign
 * Created on: 08 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */


#ifndef _DDAD_PREDICATES_2_H_
#define _DDAD_PREDICATES_2_H_

#include "Point_2.h"
#include "Segment_2.h"

class Predicates_2 {

public:


    //////////////////////////////////////////////////////////////////////
    //                      1D INTERVAL PREDICATES
    //////////////////////////////////////////////////////////////////////

    /** return true if q is in the closed interval between low and high */
    static bool in_closed_interval(int low, int high, int q);



    //////////////////////////////////////////////////////////////////////
    //                      2D SEGMENT PREDICATES
    //////////////////////////////////////////////////////////////////////

    /** return codes for Segment intersections */
    enum Seg_inter { SEG_INTER_EMPTY, SEG_INTER_OPEN_SEG, SEG_INTER_POINT };

    /**
     * Test if the interior of s1 and s2 intersect
     * This test can have three types of intersection: SEG_INTER_EMPTY means
     * that the interiors have an empty intersection;  SEG_INTER_OPEN_SEG means
     * that the segments intersect in an open segment, and SEG_INTER_POINT means
     * that the segments intersect in a point.  SEG_INTER_POINT is the generic
     * intersection.
     *
     * @degree 2
     *
     * @param s1 a segment to test
     * @param s2 the other segment to test
     * @return INTER_EMPTY if the interiors of s1 and s2 are disjoint
     *         INTER_OPEN_SEG if the intersection of the interior of
     *                  s1 and s2 is an open segment segment,
     *         INTER_POINT if the intersection of the interior of
     *                  s1 and s2 is a point,
     */
    static Seg_inter intersect_interior( const Segment_2::Std_segment& s1,
            const Segment_2::Std_segment& s2);


    //////////////////////////////////////////////////////////////////////
    //                      2D SEGMENT AND POINT PREDICATES
    //////////////////////////////////////////////////////////////////////


    /**
     * Given a segment and a point all with degree 1 coordinates
     * such that s is non-vertical, return true if p is in the
     * open halfplane above the line though s.
     *
     * @degree 2
     *
     * @param s the segment defining the open halfplane
     * @param p the point to test
     *
     * @return true if p is above
     */
    static bool above(const Segment_2::Std_segment& s,
            const Point_2::Std_point& p);


    /**
     * Given a segment and a point all with degree 1 coordinates
     * such that s is non-vertical, return true if p is in the
     * open halfplane below the line though s.
     *
     * @degree 2
     *
     * @param s the segment defining the open halfplane
     * @param p the point to test
     *
     * @return true if p is below
     */
    static bool below(const Segment_2::Std_segment& s,
            const Point_2::Std_point& p);


    /**
     * Given a segment $s$ and a point $p$ all with degree 1 coordinates
     * Test if $p$ is on the interrior of $s$, that is, $p$ is in the open
     * segment of $s$.
     *
     * @degree 2
     *
     * @param s the segment defining the open segment to test.
     * @param p the point to test
     *
     * @return true if p is on the open segment of s
     */
    static bool on_open_segment(const Segment_2::Std_segment& s,
            const Point_2::Std_point& p);


    /**
     * Given a segment $s$ and a point $p$ all with degree 1 coordinates
     * Test if $p$ is on the segment of $s$, that is, $p$ is in the open
     * segment of $s$ or $p$ is one of the end points of $s$.
     *
     * @degree 2
     *
     * @param s the segment defining the open segment to test.
     * @param p the point to test
     *
     * @return true if $p$ is on $s$
     */
    static bool on_closed_segment(const Segment_2::Std_segment& s,
            const Point_2::Std_point& p);

};

#endif
