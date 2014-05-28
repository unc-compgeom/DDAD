/**
 * Filename: Discrete_upper_envelope.h
 * Project name: degDrivenAlgoDesig
 * Created on: 21 Feb 2011
 * Author: David L. Millman <dave@cs.unc.edu> */

#ifndef _DDAD_UPPER_ENVELOPE_H_
#define _DDAD_UPPER_ENVELOPE_H_

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cmath>

#include "../base/DDAD_base.h"
#include "../base/Radix_sort.h"
#include "../base/Shuffle.h"
#include "../base/Point_2.h"
#include "../base/Line_2.h"

#include "../algos/Convex_hull.h"

static RLogChannel *upper_envelope = DEF_CHANNEL("debug/upper_envelope/shared", Log_Info);
static RLogChannel *up_env_brute = DEF_CHANNEL("debug/upper_envelope/brute", Log_Info);
static RLogChannel *up_env_orient = DEF_CHANNEL("debug/upper_envelope/orient", Log_Info);
static RLogChannel *up_env_ulogu = DEF_CHANNEL("debug/upper_envelope/ulogu", Log_Info);
static RLogChannel *up_env_ric = DEF_CHANNEL("debug/upper_envelope/ric", Log_Info);


class Discrete_upper_envelope {

public:
    typedef Line_2::Slope_and_y_intercept Line;
    /**=============================================================================
     *
     *  UPPER ENVELOPE TYPES
     *
     **===========================================================================*/

    class Envelope_cell {
    public:
        Line line;
        int left, right;
        Envelope_cell(int l=INT_MIN, int r=INT_MAX) { set_lr(l,r); }
        Envelope_cell(const Line& lne, int l, int r) { line=lne; set_lr(l,r); }
        int id() const { return line.id(); }
        void set_lr(int l, int r);
        std::string to_string() const;
        friend std::ostream&operator<<(std::ostream& out, const Envelope_cell& l) {
            return out << l.to_string();
        }
        const Line& line_op() const { return line; }
        bool operator==(const Envelope_cell& e) const {
            return left == e.left && right == e.right && line == e.line;

        }
    };


protected:

    /** Upper envelope data structure */
    typedef std::vector< Envelope_cell > Upper_envelope;

    class Building_envelope_cell {
    public:
        const Line* line;
        int left;
        Building_envelope_cell *ptr;

        Building_envelope_cell(const Line* lne=NULL, int l=INT_MIN)
            : line(lne), left(l), ptr(NULL) { }

        Building_envelope_cell(const Building_envelope_cell& b)
            : line(b.line), left(b.left), ptr(NULL) { }

        const Line& line_op() const { return *line; }
        std::string to_string(bool follow_intersections=true) const;
        friend std::ostream&operator<<(std::ostream& out,
                const Building_envelope_cell& l) {
            return out << l.to_string();
        }
    };

    /** the building envelope */
    typedef std::vector< Building_envelope_cell > Building_envelope;

    /**
     * The three sorted lists of lines data structure
     * maintains the three lists of lines that intersect a
     * building cell for the random sapling algorithm.
     * Left lines intersect the cell by intersecting it from
     * the left boundary, bottom lines intersect the cell by intersecting
     * it from below, and cell lines is the line defining the bottom
     * of the cell.
     *
     * required operations: init_iteration, least_slope_and_inc, empty
     */
    struct Three_sorted_lists_of_lines {
        Building_envelope_cell *left, *left_tail, *bottom, *cell;
        Line inf;
        Building_envelope_cell all_sorted_head;
        Building_envelope_cell *all_sorted_tail;
        Three_sorted_lists_of_lines(Building_envelope_cell& ci,
                Building_envelope_cell* left_list,
                Building_envelope_cell* left_list_tail);

        /**
         * filter the lines that intersect the building envelope
         * removing any lines that cannot part of the updated due
         * or next left list.
         */
        void filter_below();


        /**
         * filter the lines of the all sorted list
         * removing all lines that have a slope less than
         * the specified line
         */
        void filter_all_sorted(const Line& filter);


        /**
         * Init the data structure for iterating over the sorted lines.
         * In particular, set up sentials to avoid empty lists and
         * remove least sloped line l0 as there is already a cell for it.
         */
        void init_iteration();


        /**
         * assuming that the three lists are non-empty,
         * return a pointer to the line with the smallest slope from the three lists
         * and move that line's building envelope to the all sorted list
         */
        const Line* least_slope_and_inc();

        /**
         * check if the three sorted lists are empty.  This occurs
         * when all three head pointers point to the all_sorted_list head
         */
        bool empty() const;

        std::string to_string() const;
        friend std::ostream&operator<<(std::ostream& out,
                const Three_sorted_lists_of_lines& l) {
            return out << l.to_string();
        }
    };

    /**
     * The one sorted lists of lines data structure
     * maintains the iterators for traversing a set of lines.
     * sorted by slope.  This class exists so that we can
     * have the same interface to ulogu DUE algo
     * for when we are only running the ulogu algorithm
     * or when we are running it as part of the ric algo.
     *
     * required operations: init_iteration, least_slope_and_inc, empty
     */
    struct One_sorted_list_of_lines {
        std::vector< const Line* >::const_iterator begin, end, cur;
        One_sorted_list_of_lines(const std::vector< const Line* >& l)
            : begin(l.begin()), end(l.end()) { }
        std::string to_string() const { return DDAD_util::vect_of_ptrs_to_string(begin, end); }

        /**
         * Init the data structure for iterating over the sorted lines.
         * In particular, the current iterator to the begin iterator.
         */
        void init_iteration() { cur = begin; }

        /**
         * assuming that the three lists are non-empty,
         * return a pointer to the line with the smallest slope.
         * In particular, return the line at cur and advance.
         */
        const Line* least_slope_and_inc();

        /** check the iteration is out of lines.  This happens when cur==end */
        bool empty() const { return cur == end; }

        friend std::ostream&operator<<(std::ostream& out,
                const One_sorted_list_of_lines& l) {
            return out << l.to_string();
        }
    };


    /** the beginning index for the due */
    int one_;

    /** the size of the discrete upper envelope from [one_..U] */
    int U_;

    /** the upper envelope */
    Upper_envelope envelope_;

public:

    // export iterators over the envelope
    typedef Upper_envelope::iterator iterator;
    typedef Upper_envelope::const_iterator const_iterator;

    // export tmp data types
    typedef std::vector< const Line* > Sorted_lines_mem;
    static int sorted_lines_mem_size(int n) { return n; }

    typedef std::vector< Building_envelope_cell > Rand_order_mem;
    static int rand_order_mem_size(int n) { return n; }

    typedef Building_envelope Building_envelope_mem;
    static int building_envelope_mem_size(int n) { return n + 2; }

    typedef std::vector< Point_2::Std_point > Building_with_orient_mem;
    static int building_with_orient_mem_size(int n) { return n + 2; }


protected:
    /**===========================================================================
     *
     *  UPPER ENVELOPE OPERATIONS
     *
     **=========================================================================*/

    /** return true if l1 and l2 have different slopes */
    bool line_slope_neq(const Line& l1, const Line& l2) const {
        return Line_2::slope_order(l1,l2) != Line_2::SLOPE_ORDER_SAME;
    }

    /** return true if slope(l1) < slope(l2) */
    static bool line_slope_lt(const Line& l1, const Line& l2) {
        return Line_2::slope_order(l1,l2) == Line_2::SLOPE_ORDER_LESS;
    }

    /** return true if y-intercept of l1 is less than y-intercept of l2. */
    bool line_y_int_lt(const Line& l1, const Line& l2) const {
        return Line_2::y_intersection_order(l1,l2,0) == Line_2::Y_ORDER_BELOW;
    }

    /** return true if l1 and l2 intersect on x = x0 */
    bool line_above(const Line& l1, const Line& l2, int x0) const {
        return Line_2::y_intersection_order(l1, l2, x0)
            == Line_2::Y_ORDER_ABOVE;
    }

    /** return true if l1 is strictly below l2 on line y */
    bool line_below(const Line& l1, const Line& l2, int x0) const {
        return Line_2::y_intersection_order(l1, l2, x0)
            == Line_2::Y_ORDER_BELOW;
    }

    /** return true if l1 and l2 intersect on x = x0 */
    bool line_intersect(const Line& l1, const Line& l2, int x0) const {
        return Line_2::y_intersection_order(l1, l2, x0)
            == Line_2::Y_ORDER_COINCIDENT;
    }

    /** return true if l1 is above or intersects l2 at x0 */
    bool line_above_or_on(const Line& l1, const Line& l2, int x0) const {
        Line_2::Y_order yorder = Line_2::y_intersection_order(l1, l2, x0);
        return yorder == Line_2::Y_ORDER_ABOVE
            || yorder == Line_2::Y_ORDER_COINCIDENT;
    }

    /**
     * binary search the interger coordinates [alpha, alpha+1)
     * bounding the intersection of the lines l and h in the interval low
     * and high.
     * Return the lower bound alpha of the interval, the upper bound is assumed
     * to be \alpha + 1.  Return -1 if the lines do not intersect in the interval.
     */
    int bin_search_intersection(const Line& ll, const Line& hh, int low, int high);

    /**
     * Given a partially built envelope from (-inf,one), called envelope,
     * and a set of unquely sloped lines sorted by slope, such that the least
     * sloped line of L has a larger slope than the greatest sloped line
     * of the envelope, compute the DUE of the lines in L and the lines defining
     * envelope from (-inf, U].
     *
     * L should be in a structure suppoerting iteration by the following
     * commands:
     *  init_iteration: sets up iterating over the list
     *  least_slope_and_inc: returs the next least sloped line and incraments
     *          for the next iteration.
     *  empty: returns if all lines have be iterated over.
     *
     * This operation used degree 2 and O(|L|*log U) time.
     * @param envelope to build from and store into
     * @param L the sorted lines
     * @param one beginning of the build region (inclusive)
     * @param U the end of the build region (inclusive)
     */
    template< typename Envelope, typename SortedLines >
    void due_w_bin_search_of_sorted_and_filtered_lines(Envelope& envelope,
            SortedLines& L, int one, int U);


    /**
     * Sort the lines, then iterate over them,
     * if any two lines have the same slope,
     * select only the line with the larger y-intercept value
     * return the list of remaining sorted lines
     */
    template< class OutputLines, class InputLines >
    void sort_and_filter_by_slope(OutputLines out,
            InputLines begin, InputLines end) const;


    /**
     * init the one and UB variables, verify the input contains some points
     * and the LB and UB variables are 1 < LB < UB.
     */
    template < typename Lines >
    void init_build_and_verify_input(const Lines& input, int LB, int UB);


    /**
     * build using the ric alg assuming the input sites are sorted and have
     * unique slope.
     */
    void build_ric_with_slope_unique_lines_sorted_by_slope(
            std::list< const Line* >& sorted_lines,
            Rand_order_mem& rand_order,
            Building_envelope_mem& building_envelope01,
            Building_envelope_mem& building_envelope02);


    /**
     * build using the u log u algorithm assuminng the input lines are
     * sorted and have unique slope and memory is allocated
     */
    void build_ulogu_with_slope_unique_lines_sorted_by_slope(
            const std::vector< const Line* >& sorted_lines,
            Building_envelope_mem& build_env);


    /**
     * build using the deg 3 algorithm assuming that the input lines
     * are sorted and have unique slope and memory is allocated
     */
    void build_orient_with_slope_unique_lines_sorted_by_slope(
            const std::vector< const Line* >& sorted_lines,
            Building_with_orient_mem& dual_points);



    /** convert the building envelope to an envelope */
    void building_envelope_to_envelope(Upper_envelope& envelope,
            const Building_envelope& build_env) {
        Building_envelope::const_iterator convert_begin = build_env.begin()+1;
        Building_envelope::const_iterator convert_end = build_env.begin()+(build_env.size()-1);
        for (Building_envelope::const_iterator c_i = convert_begin ; c_i != convert_end ; ++c_i) {
            envelope.push_back(Envelope_cell(*(c_i->line), c_i->left, (c_i+1)->left-1));
        }
    }


public:

    Discrete_upper_envelope() : one_(0), U_(0) { }

    /** Compute the upper envelope of a set of lines  */
    void build(const std::vector< Line >& input, int UB);

    /** Compute the upper envelope of a set of lines with RIC construction */
    void build_ric(const std::vector< Line >& input, int LB, int UB);

    /**
     * Compute the upper envelope of a set of lines with an algorithm simmilar
     * to build_orient, only replacing the orientation test with a binary search
     * for slabs containing intersections
     */
    void build_ulogu(const std::vector< Line >& input, int LB, int UB);

    /** Compute the upper envelope of a set of line with orientation predicate */
    void build_orient(const std::vector< Line >& input, int LB, int UB);

    /** Compute the upper envelope of a set of lines with brute force */
    void build_brute_force(const std::vector< Line >& input, int LB, int UB);

    /**
     * Compute the discrete upper envelope of a set of lines using the ric
     * algorithm were no two lines
     * have the same slope and the lines are already sorted by slope in
     * increasing order.
     */
    void build_ric_with_slope_unique_lines_sorted_by_slope(
            const std::vector< Line >& input, int UB,
            Rand_order_mem& rand_order,
            Sorted_lines_mem&,
            Building_envelope_mem& building_envelope01,
            Building_envelope_mem& building_envelope02);

    /**
     * Compute the discrete upper envelope of a set of lines using the
     * u log u algorithm where no two lines have the same slope and
     * the lines are already sorted by slope in increasing order.
     */
    void build_ulogu_with_slope_unique_lines_sorted_by_slope(
            const std::vector< Line >& input, int UB,
            Sorted_lines_mem& sorted_lines,
            Building_envelope_mem& build_env);

    /**
     * Compute the upper envelope of a set of lines with the
     * degree 3 orientation predicate.  Assume that no two lines
     * have the same slope and the liens are already sorted by
     * slope in increasing order.
     */
    void build_orient_with_slope_unique_lines_sorted_by_slope(
            const std::vector< Line >& input, int UB,
            Sorted_lines_mem& sorted_lines,
            Building_with_orient_mem& dual_points);


    /** return the size of the grid */
    int U() const { return U_; }

    /** return the one of the grid */
    int one() const { return one_; }

    /** begin iterator over the envelope */
    Upper_envelope::const_iterator begin() const { return envelope_.begin(); }

    /** end iterator over the envelope */
    Upper_envelope::const_iterator end() const { return envelope_.end(); }

    /** return the number of cells in the discrete upper envelope */
    int size() const { return envelope_.size(); }

    /** clear the data of the discrete upper envelope */
    void clear() { envelope_.clear(); }

    /**
     * given the set of lines used to build the discrete upper envelope, verify
     * using brute force that this envelope is correct.
     */
    bool verify(const std::vector< Line >& input);

    /** return the string represenation of the upper envelope */
    std::string to_string() const;

    /** Output operator */
    friend std::ostream& operator<<(std::ostream& out,
            const Discrete_upper_envelope& e) {
        return out << e.to_string();
    }
};

#endif
