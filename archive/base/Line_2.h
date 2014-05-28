/**
 * Filename: Line_2.h
 * Project name: degDrivenAlgoDesign
 * Created on: 27 Jun 2011
 * @author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_LINE_2_H_
#define _DDAD_LINE_2_H_

#include <climits>

#include "DDAD_base.h"
#include "Point_2.h"

class Line_2 {
public:

    //////////////////////////////////////////////////////////////////////
    //                      2D LINE REPRESENTATIONS
    //////////////////////////////////////////////////////////////////////

    /**
     * A line representation defined by a slope and a y-intercept of the
     * form y = mx + b, where m is the slope and b is the y intercept.
     */
    class Slope_and_y_intercept {
    friend class Line_2;
    protected:
        int slope_;
        int y_int_;
        int id_;
        int m() const { return slope_; }
        int b() const { return y_int_; }
        int eval(int x) const {
            // freeman - need to cast because sqrt is overloaded for many types
            //rAssert(-std::sqrt(-(INT_MIN>>2)) < m());
            //rAssert(m() < std::sqrt(INT_MAX));
            //rAssert(-std::sqrt(-(INT_MIN>>2)) < x);
            //rAssert(x < std::sqrt(INT_MAX));
            rAssert(-std::sqrt(static_cast<float>(-(INT_MIN>>2))) < m());
            rAssert(m() < std::sqrt(static_cast<float>(INT_MAX)));
            rAssert(-std::sqrt(static_cast<float>(-(INT_MIN>>2))) < x);
            rAssert(x < std::sqrt(static_cast<float>(INT_MAX)));
            rAssert(INT_MIN>>1 < b());
            rAssert(b() < INT_MAX>>1);
            return m()*x + b(); }
    public:
        Slope_and_y_intercept(int slope=0, int y_int=0, int id=-1)
            : slope_(slope), y_int_(y_int),id_(id) {}
        Slope_and_y_intercept(const Slope_and_y_intercept& l)
            : slope_(l.slope_), y_int_(l.y_int_), id_(l.id_) {}

        std::string to_string() const {
            std::ostringstream os;
            if (id() >= 0) { os << id() << ": "; }
            os << "y = " << m() << "x + " << b();
            return os.str();
        }

        /** reset the values of the line */
        void reset(int slope, int y_int, int id=-1) {
            slope_ = slope; y_int_ = y_int; id_ = id;
        }

        /** return the id of the line */
        int id() const { return id_; }

        /** return true if the lines are the same (ignorming id) */
        bool operator==(const Slope_and_y_intercept& l) const {
            return m() == l.m() && b() == l.b();
        }

        /** return true if the lines are different (ignoring id) */
        bool operator!=(const Slope_and_y_intercept& l) const {
            return !(*this == l);
        }

        /** output operator for the line */
        friend std::ostream& operator<<(std::ostream& o,
                const Slope_and_y_intercept& l) {
            return o << l.to_string();
        }
    };

    class Vertical {
    friend class Line_2;
    protected:
        int x_int_;
        int id_;
        int x_int() const { return x_int_; }
    public:
        Vertical(int x_int=0, int id=-1) : x_int_(x_int),id_(id) {}
        std::string to_string() const {
            std::ostringstream os;
            if (id() >= 0) { os << id() << ": "; }
            os << "y = " << x_int();
            return os.str();
        }
        int id() const { return id_; }
        void set_x_int(int xint) { x_int_ = xint; }
        friend std::ostream& operator<<(std::ostream& o,
                const Vertical& l) {
            return o << l.to_string();
        }
    };



    //////////////////////////////////////////////////////////////////////
    //                      2D LINE PREDICATES
    //////////////////////////////////////////////////////////////////////
    enum X_order { X_ORDER_LEFT, X_ORDER_RIGHT, X_ORDER_COINCIDENT };


    /**
     * Given three lines l0, l1 and l2 such that l0 != l1, l0 != l2
     * and neither l1 nor l2 are parallel to l0,
     * return the x ordering of the intersections of l1 and l2 along
     * l0.
     *
     * That is, return:
     *  X_ORDER_LEFT if $(l1 \cap this).x < (l2 \cap this).x$
     *  X_ORDER_RIGHT if $(l2 \cap this).x < (l1 \cap this).x$
     *  X_ORDER_COINCIDENT if $(l1 \cap l2).x \in this$
     *
     * @param l0 one of the test lines
     * @param l1 one of the test lines
     * @param l2 the other test line
     *
     */
    static X_order x_intersection_order(
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1,
            const Slope_and_y_intercept& l2);


    /**
     * Given lines l0, l1, l2 and l3 such that l0 != l1, and l2 != l3
     * with l0 and l1 non-parallel and l2 and l3 non-parallel ,
     * return the x ordering of the intersections of l1 and l2 and
     * l2 and l3
     *
     * That is, return:
     *  X_ORDER_LEFT if $(l0 \cap l1).x < (l2 \cap l3).x$
     *  X_ORDER_RIGHT if $(l2 \cap l3).x < (l0 \cap l1).x$
     *  X_ORDER_COINCIDENT if $(l0 \cap l1).x == (l2 \cap l3).x$
     *
     * @param l0 for l0 \cap l1
     * @param l1 for l0 \cap l1
     * @param l2 for l2 \cap l3
     * @param l3 for l2 \cap l3
     */
    static X_order x_intersection_order(
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1,
            const Slope_and_y_intercept& l2,
            const Slope_and_y_intercept& l3);




    enum Y_order { Y_ORDER_BELOW, Y_ORDER_ABOVE, Y_ORDER_COINCIDENT };

    /**
     * Given two lines lines l0 and l1 return the y ordering of the
     * intersection of l0 and l1 with the vertical line l:x = x0
     *
     * That is, return:
     *   Y_ORDER_BELOW if $(l0 \cap l).y < (l1 \cap l)$
     *   Y_ORDER_ABOVE if $(l0 \cap l).y > (l1 \cap l)$
     *   Y_ORDER_COINCIDENT if $(l0 \cap l).y == (l1 \cap l)$
     *
     * @param l0 for l0 \cap l
     * @param l1 for l1 \cap l
     * @return the y ordering of the l0 \cap l and \l1 \cap l
     */
    static Y_order y_intersection_order(
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1,
            int x0);



    enum Cell_inter_type { CELL_INTER_EMPTY, CELL_INTER_INTERIOR,
        CELL_INTER_LEFT_BDD,  CELL_INTER_LOWER_BDD,
        CELL_INTER_RIGHT_BDD, CELL_INTER_RIGHT_AND_LEFT_BDD };


    /**
     * Given a query line lines l and a cell defined by
     * tuple (m, left,right), such that the cell consists of all points
     * on or above the line m in the vertical slab [left,right],
     * return the intersection type of l with the cell.
     *
     * That is:
     *  CELL_INTER_EMPTY if the line is disjoint from the cell.
     *  CELL_INTER_INTERIOR intersects the cell and passes only though
     *          the vertical defining the cell
     *  CELL_INTER_LEFT_BDD intersects lower bounding line on the left side
     *          of the cell.
     *  CELL_INTER_RIGHT_BDD intersects lower bouding line on the right side
     *          of the cell.
     *  CELL_INTER_RIGHT_AND_LEFT_BDD when a cell is 1 pixel wide a line can
     *          intersect the left and right at the same time.
     *  CELL_INTER_LOWER_BDD the line intersects the bounding not on the left or
     *          right boundary.
     *
     * @param l the query line
     * @param m the line defing the cell
     * @param a the left boundary of the cell
     * @param b the right boundary of the cell
     * @return the intersection type of the line and the cell
     */
    static Cell_inter_type classify_intersection(const Slope_and_y_intercept& l,
            const Slope_and_y_intercept& m, int left, int right);

    /**
     * Return true if the classification of the cell is CELL_INTER_INRERIOR.
     * This doesn't call classify_intersection and should be faster.
     */
    static bool interior_intersection(const Slope_and_y_intercept& l,
            const Slope_and_y_intercept& m, int left, int right);


    /**
     * Given lines l0, l1, l2 and l3 such that l0 != l1, and l2 != l3
     * with l0 and l1 non-parallel and l2 and l3 non-parallel ,
     * return the y ordering of the intersections of l1 and l2 and
     * l2 and l3
     *
     * That is, return:
     *  Y_ORDER_BELOW if $(l0 \cap l1).y < (l2 \cap l3).y$
     *  Y_ORDER_ABOVE if $(l2 \cap l3).y < (l0 \cap l1).y$
     *  Y_ORDER_COINCIDENT if $(l0 \cap l1).y == (l2 \cap l3).y$
     *
     * @param l0 for l0 \cap l1
     * @param l1 for l0 \cap l1
     * @param l2 for l2 \cap l3
     * @param l3 for l2 \cap l3
     */
    static Y_order y_intersection_order(
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1,
            const Slope_and_y_intercept& l2,
            const Slope_and_y_intercept& l3);


    /**
     * Given two lines l0 and l1 such that l0 != l1, return the
     * y ordering of l1 and l2 along the left boundary of problem
     * (assuming the boundary is at infinite).
     *
     * That is, return:
     *  Y_ORDER_BELOW if the slope of l0 is greater than the slope of l1
     *          or their slopes are the same and the y-intercept of
     *          l0 is less than l1.
     *  Y_ORDER_ABOVE if the slope of l1 is less than the slope of l0
     *          or their slopse are the same and the y-intercept of
     *          l1  greater than l0.
     *  Y_ORDER_COINCIDENT is not returned this would violate the
     *  precondition of l0 != l1.
     *
     * @param l0 a test line
     * @param l1 another test line
     */
    static Y_order y_order_on_left_boundary(
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1);


    /**
     * Given two lines l0 and l1 such that l0 != l1, return the
     * y ordering of l1 and l2 along the right boundary of problem
     * (assuming the boundary is at infinite).
     *
     * That is, return:
     *  Y_ORDER_BELOW if the slope of l0 is less than the slope of l1
     *          or their slopes are the same and the y-intercept of
     *          l0 is less than l1.
     *  Y_ORDER_ABOVE if the slope of l1 is less than the slope of l0
     *          or their slopse are the same and the y-intercept of
     *          l1  less than l0.
     *  Y_ORDER_COINCIDENT is not returned this would violate the
     *  precondition of l0 != l1.
     *
     * @param l0 a test line
     * @param l1 another test line
     */
    static Y_order y_order_on_right_boundary(
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1);



    enum Slope_order { SLOPE_ORDER_LESS, SLOPE_ORDER_GREATER, SLOPE_ORDER_SAME };

    /**
     * Given two lines lines l0 and l1 return their slope ordering
     *
     * That is, return:
     *   SLOPE_ORDER_LESS if slope(l0) < slope(l1)
     *   SLOPE_ORDER_GREATER if slope(l0) > slope(l1)
     *   SLOPE_ORDER_SAME if slope(l0) == slope(l1)
     *
     * @param l0 for slope comparison
     * @param l1 for slope comparison
     * @return the slope ordering of l0 and l1
     */
    static Slope_order slope_order(
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1);



    enum Parallel { IS_NOT_PARALLEL, IS_PARALLEL };

    /**
     * return true if the two lines are parallel
     * @param l0 one of the test lines
     * @param l1 the other test line
     */
    static Parallel is_parallel(const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1);



    /**
     * return the coordinates of the intersection of l0 and l1
     * @param x x coordinate of intersection point
     * @param y y coordinate of intersection point
     * @param l0 one of the test lines
     * @param l1 the other test line
     */
    static Parallel intersection_coords(float& x, float& y,
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1);


    /**
     * return the coordinates of the intersection of l0 and l1
     * @param x x coordinate of intersection point
     * @param y y coordinate of intersection point
     * @param l0 slope-y-intercept line
     * @param l1 vertical line
     */
    static Parallel intersection_coords(float& x, float& y,
            const Slope_and_y_intercept& l0,
            const Vertical& l1);



    /**
     * compute the coordinates of the grid line to the left of the inersction
     * of l0 and l1, i.e. \floor( (l0 \cap l1).x ).  Set this value to x.
     * If the lines intersect then return IS_NOT_PARALLEL and IS_PARALLEL if
     * they do intersect.
     *
     * @param x the vertical grid line left of the intersection of l0 and l1
     * @param l0 slope-y-intercept line
     * @param l1 slope-y-intercept line
     * @return IS_PARALLEL if l0 || l1 and IS_NOT_PARALLEL if they intersect.
     */
    static Parallel vertical_grid_line_left_or_thourgh_intersection(int& x,
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1);


    /**
     * given a line l0 return the point dual to the line using the
     * transformation y = mx + b -->  (m,-b).  If no id is specified
     * the id of the line is used.  The data pointer of the line is set
     * to point to l0.
     *
     * @param the line to convert to a point
     * @param to set the id (if id is not specified id of l0 is used)
     * @param the point dual to this line
     */
    static Point_2::Std_point dual(const Slope_and_y_intercept& l0, int id=-1);



    /**
     * construct the slope of the line
     *
     * @param l0 the line in which to construct the slope.
     * @return the slope of the line
     */
    static int slope(const Slope_and_y_intercept& l0);

    /**
     * construct the y intercept of the line
     *
     * @param l0 the line in which to construct the y-intercept of.
     * @return the y intercept of the line
     */
    static int y_intercept(const Slope_and_y_intercept& l0);
};


#endif

