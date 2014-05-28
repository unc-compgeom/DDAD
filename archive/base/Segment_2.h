/**
 * Filename: Segment_2.h
 * Project name: degDrivenAlgoDesign
 * Created on: 08 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */


#ifndef _DDAD_SEGMENT_2_H_
#define _DDAD_SEGMENT_2_H_

#include <iostream>
#include <sstream>

#include "DDAD_base.h"
#include "Point_2.h"


/**
 * Define Predicates_2 so that we can give it access to Segment's coordinate
 * representation */
class Predicates_2;

/**
 * A 2 dimensional segment with identification number.  The
 * identification numer is used when it is nonnegative and ignored when
 * negative
 */
class Segment_2 {
public:

    //////////////////////////////////////////////////////////////////////
    //                      2D SEGMENT REPRESENTATIONS
    //////////////////////////////////////////////////////////////////////

    /** A segment with degree 1 coordinates. */
    class Std_segment {
    friend class Predicates_2;
    protected:
        Point_2::Std_point p1_, p2_;
        int id_;
   public:

        /**
         * Construct a segment from points with degree 1 coordinates.  A point
         * may have a label, if negative, it is ignored.
         *
         * @param p1 a point defining the segment, defaults to Std_point().
         * @param p2 a point defining the segment, defaults to Std_point().
         * @param id id of the segment, ignored if negative, default -1.
         */
        Std_segment(const Point_2::Std_point& p1 = Point_2::Std_point(),
                const Point_2::Std_point& p2 = Point_2::Std_point(), int id=-1)
            : p1_(p1), p2_(p2), id_(id) { rAssert(p1 != p2); }

        /** Copy construcor for a segment */
        Std_segment(const Std_segment& s) : p1_(s.p1_), p2_(s.p2_), id_(s.id_) {}

        /** return the string representation of the segment */
        std::string to_string() const;

        /** return the id of the segment */
        int id() const { return id_; }


        /**
         * A standard segment is defined by two standard points.
         * This function gives access the to the first point defining the
         * segment.  Note that there is no assumed ordering to the points
         * defing a segment.
         * @return a constant reference to the first point defining a segment
         */
        const Point_2::Std_point& p1() const { return p1_; }

        /**
         * A standard segment is defined by two standard points.
         * This function gives access the to the second point defining the
         * segment.  Note that there is no assumed ordering to the points
         * defing a segment.
         * @return a constant reference to the second point defining a segment
         */
        const Point_2::Std_point& p2() const { return p2_; }

        /** return true if the segments are the same
         * (ignorming id and point ordering)
         */
        bool operator==(const Std_segment& s) const {
            return (p1() == s.p1() && p2() == s.p2())
                || (p1() == s.p2() && p2() == s.p1());
        }

        /**
         * return true if the points have different coordinate
         * values (ignoring i and point ordering)
         */
        bool operator!=(const Std_segment& s) const {
            return !(*this == s);
        }

        /** output operator for the line */
        friend std::ostream& operator<<(std::ostream& o,
                const Std_segment& s) {
            return o << s.to_string();
        }
    };
};

#endif
