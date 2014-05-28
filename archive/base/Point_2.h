/**
 * Filename: Point_2.h
 * Project name: degDrivenAlgoDesign
 * Created on: 12 Nov 2010
 * @author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_POINT_2_H_
#define _DDAD_POINT_2_H_

#include <iostream>
#include <sstream>

#include "DDAD_base.h"
#include "Algebraic_predicates.h"

/**
 * Define Predicates_2 so that we can give it access to Point's coordinate
 * representation */
class Predicates_2;

/**
 * A 2 dimensional point with identification number.  The
 * identification numer is used when it is nonnegative and ignored when
 * negative */
class Point_2 {
public:
    //////////////////////////////////////////////////////////////////////
    //                      2D POINT REPRESENTATIONS
    //////////////////////////////////////////////////////////////////////

    /** A point with degree 1 coordinates. */
    class Std_point {
    friend class Point_2;
    friend class Predicates_2;
    friend class Constructions_2;
    protected:
        int x_, y_, id_;
        void* data_;
        int x() const { return x_; }
        int y() const { return y_; }
    public:

        /**
         * Construct a point from its degree 1 Cartisian coordinates.
         * A point may have my have an id.  If the id is negative the id is
         * ignored.
         *
         * @param x the x-coordinate of the point, defaults to 0
         * @param y the y-coordinate of the point, defaults to 0
         * @param id the id of the point, ignored if negative, defaults to -1
         */
        Std_point(int x=0, int y=0, int id=-1, void* data=NULL)
            : x_(x), y_(y), id_(id), data_(data) {}

        /** Copy constructor for a point */
        Std_point(const Std_point& p)
            : x_(p.x_), y_(p.y_), id_(p.id_), data_(p.data_) {}

        /** Return the string representation of the point */
        std::string to_string() const;

        /** return the id of the point */
        int id() const { return id_; }

        /** set the data pointer */
        void set_data(void* data) { data_ = data; }

        /** get the data pointer */
        const void* data() const { return data_; }

        /** assignment operator */
        Std_point& operator=(const Std_point& p) {
            if (this == &p) { return *this; }
            x_ = p.x_;
            y_ = p.y_;
            id_ = p.id_;
            data_ = p.data_;
            return *this;
        }

        /** return true if the points are the same (ignorming id) */
        bool operator==(const Std_point& p) const {
            return x() == p.x() && y() == p.y();
        }

        /**
         * return true if the points have different coordinate
         * values (ignoring id)
         */
        bool operator!=(const Std_point& p) const {
            return !(*this == p);
        }

        /** output operator for the line */
        friend std::ostream& operator<<(std::ostream& o,
                const Std_point& p) {
            return o << p.to_string();
        }
    };



    //////////////////////////////////////////////////////////////////////
    //                      2D POINT PREDICATES
    //////////////////////////////////////////////////////////////////////




    enum Orientation { ORIENT_LEFT, ORIENT_RIGHT, ORIENT_COLINE };

    /** return the string representation of the orientation */
    static std::string orient_to_string(const Orientation& orient) {
        switch (orient) {
            case ORIENT_LEFT: return "ORIENT_LEFT";
            case ORIENT_RIGHT: return "ORIENT_RIGHT";
            case ORIENT_COLINE: return "ORIENT_COLINE";
            default: throw DDAD::Case_not_covered_error();
        }
    }

    /** Output operator for the orientation return type*/
    friend std::ostream& operator<<(std::ostream& out, const Orientation& o) {
        return out << orient_to_string(o);
    }

    /** Given three distinct points p, q, and r each with degree 1 coordinates,
     * return the orientation of the straitline path from p to q to r.
     *
     * @degree 2
     *
     * @param p the 1st point of the path
     * @param q the 2nd point of the path
     * @param r the 3rd point of the path
     * @return the paths orientation
     */
    static Orientation orientation(const Point_2::Std_point& p,
            const Point_2::Std_point& q, const Point_2::Std_point& r);





    enum Closer { CLOSER_TO_FIRST, CLOSER_TO_SECOND, CLOSER_EQUIDISTANT };

    /** return the string representation of the closer return value */
    static std::string closer_to_string(const Closer& closer) {
        switch(closer) {
            case CLOSER_TO_FIRST: return "CLOSER_TO_FIRST";
            case CLOSER_TO_SECOND: return "CLOSER_TO_SECOND";
            case CLOSER_EQUIDISTANT: return "CLOSER_EQUIDISTANT";
            default: throw DDAD::Case_not_covered_error();
        }
    }

    /** Output operator for the closer type */
    friend std::ostream& operator<<(std::ostream& out, const Closer& c) {
        return out << closer_to_string(c);
    }

    /** given three points a, b and q, each with degree 1 coordinates,
     * return whether q is closer to a, b or equidistant to both.
     *
     * @degree 2
     *
     * @param a the first point
     * @param b the second point
     * @param q the query point
     * @param return CLOSER_TO_FIRST if |q-a| < |q-b|,
     *          CLOSER_TO_SECOND if |q-a| > |q-b|,
     *          CLOSER_EQUIDISTANT if |q-a| = |q-b|,
     */
    static Closer closer(const Point_2::Std_point& a,
            const Point_2::Std_point& b, const Point_2::Std_point& q);





    /**
     * Given two points with degree 1 coordinates,
     * return true if the points are on a vertical line
     *
     * @degree 1
     *
     * @param p a point to test
     * @param q the other point to test
     */
    static bool on_vertical_line(const Point_2::Std_point& p,
            const Point_2::Std_point& q);





    /**
     * Given two points with degree 1 coordinates,
     * return true if p precedes q, that is p.x < q.x
     * or p.x==q.x and p.y < q.y.
     *
     * @degree 1
     *
     * @param p a point to test
     * @param q the other point to test
     */
    static bool precedes(const Point_2::Std_point& p, const Point_2::Std_point& q);




};


#endif /* POINT_2_H_ */
