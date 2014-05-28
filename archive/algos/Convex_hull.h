/**
 * Filename: Convex_hull.h
 * Project name: degDrivenAlgoDesig
 * Created on: 22 Feb 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_CONVEX_HULL_H_
#define _DDAD_CONVEX_HULL_H_

#include <algorithm>
#include <list>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"
#include "../base/Poly_chain_2.h"
#include "../base/Predicates_2.h"

static RLogChannel *convex_hull
        = DEF_CHANNEL("debug/convex_hull", Log_Info);
static RLogChannel *graham_yao
        = DEF_CHANNEL("debug/convex_hull/graham_yao", Log_Info);

class Convex_hull {

public:
    typedef Point_2::Std_point Point;
    typedef std::vector< Point > Sites;

protected:

    /** Set of points in which we computed the convex hull */
    Sites sites_;

    /** The convex hull of S */
    Poly_chain_2 ch_;

    /** The size of the grid on which we computed the convex hull */
    int U_;

    /** the one value for the problem */
    const static int one_ = 1;

public:

    // an algorithm for more easily running graham yao
    // on a sorted set of points.
    // the algorithm is broken up into parts so that it may be used
    // within other algorithms.
    //
    // There are invarients to the operations, but preconditions are not
    // always checked, use with caution.
    struct Graham_yao {

        Poly_chain_2::Vertex_list::const_reverse_iterator from, to;

        /**
         * init the construction with p0 and p1
         * If we do a x_low to x_high construction of the lower hull,
         * than p0.x() < p1.x.
         * Or for a y_low to y_high construction of the right hull
         * than p0.y < p1.y.  etc
         *
         * For flexibility, these invarients are not checked.
         */
        void init(Poly_chain_2& chain,
                const Point& p0, const Point& p1);

        /**
         * Helper function for add_cw and add_ccw.
         * Assuming that all ordered triples in the poly chain form a turn
         * of dir D, add point q to the chain, and if invarient is broken,
         * resotre it.  The flag keep colinear specifies if colinear points
         * should are kept.  Buy default it is set to not not keep them.
         * Please, use caution when enabling this flag, it is NOT
         * orient_or_ahead, it WILL add any colinear point.
         *
         * For flexibility, invarients are not checked.
         */
        void add_helper(Poly_chain_2& chain, const Point& q,
                const Point_2::Orientation& orient,
                bool keep_colinear=false);

        /**
         * Assuming that all ordered triples in the poly chain form a
         * right turn, add the point p to the poly chain, and if it is
         * broken, resotre the invarent.
         *
         * For flexibility, invarients are not checked.
         */
        void add_cw(Poly_chain_2& chain, const Point& q) {
            add_helper(chain, q, Point_2::ORIENT_RIGHT);
        }

        /**
         * Assuming that all ordered triples in the poly chain form a
         * left turn, add the point p to the poly chain, and if it is
         * broken, resotre the invarent.
         *
         * For flexibility, invarients are not checked.
         */
        void add_ccw(Poly_chain_2& chain, const Point& q) {
            add_helper(chain, q, Point_2::ORIENT_LEFT);
        }

        /**
         * Assuming that all ordered triples in the poly chain form a
         * left turn or are ahead, add the point p to the poly chain, and if it is
         * broken, resotre the invarent.
         *
         * For flexibility, invarients are not checked.
         */
        void add_ccw_or_coline(Poly_chain_2& chain, const Point& q) {
            add_helper(chain, q, Point_2::ORIENT_LEFT, true);
        }

        /**
         * Compute the upper hull of a set of sites p, assuming that
         * the sites are sorted by <.
         *
         * For flexibility, the invarients are not checked
         */
        void build_upper_hull(Poly_chain_2& chain, const Sites& P);

        /**
         * Compute the lower hull of a set of sites p, assuming that
         * the sites are sorted by <.
         *
         * For flexibility, the invarients are not checked
         */
        void build_lower_hull(Poly_chain_2& chain, const Sites& p);
    };


    /**
     * Given a set of points S that lay in a box
     * from 1 to UB build the convex hull of S
     * @param S is a set of point in [1..UB]^2
     * @param UB is a positive integer greater than 1
     */
    void build(const Sites& input, int UB);

    /** return the one of the problem */
    int one() const { return one_; }

    /** Return the U of the problem */
    int U() const { return U_; }

    /** Return the number of vertices of the convex hull */
    int N() const { return ch_.num_vertices(); }

    /** return the polygon representing the convex hull of the sites */
    const Poly_chain_2& polygon() const { return ch_; }

    /** begin iterator over the vertices of the hull */
    Poly_chain_2::Vertex_list::const_iterator vertices_begin() const {
        return ch_.vertices_begin();
    }

    /** end iterator vertices of the hull  */
    Poly_chain_2::Vertex_list::const_iterator vertices_end() const {
        return ch_.vertices_end();
    }

    /** begin iterator over the sites */
    Sites::const_iterator sites_begin() const { return sites_.begin(); }

    /** end iterator over the sites */
    Sites::const_iterator sites_end() const { return sites_.end(); }

    /** Return the string representation of the convex hull */
    std::string to_string() const;

    /** output operator */
    friend std::ostream& operator<<(std::ostream& out,
            const Convex_hull& c) {
        return out << c.to_string();
    }

};

#endif
