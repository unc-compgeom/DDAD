/**
 * Filename: 22 Feb 2011
 * Project name: degDrivenAlgoDesig
 * Created on: 22 Feb 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "Convex_hull.h"

void Convex_hull::Graham_yao::init(Poly_chain_2& chain,
        const Point& p0, const Point& p1) {
    chain.append(p0);
    chain.append(p1);
    from = chain.vertices_rbegin();
    ++from;
    to = chain.vertices_rbegin();
}


void Convex_hull::Graham_yao::add_helper(Poly_chain_2& chain,
        const Point& q, const Point_2::Orientation& orient,
        bool keep_colinear) {

    // print the chain before adding
    rLog(graham_yao, "Pre-insert, point: %s \n%s\tFrom: %s To: %s",
            thing_to_cstring(q),
            vect_to_cstring(chain.vertices_begin(), chain.vertices_end()),
            thing_to_cstring(*from), thing_to_cstring(*to));

    // pop off all points that break the orientation invarient
    while (from != chain.vertices_rend()
            && (Point_2::orientation(*from, *to, q) != orient
                && (!keep_colinear
                    || Point_2::orientation(*from, *to, q) != Point_2::ORIENT_COLINE)) ) {
        rLog(graham_yao, "%s Poping: %s",
                thing_to_cstring(Point_2::orientation(*from, *to, q)),
                thing_to_cstring(chain.back()));

        // pop the last element and reset the pointers
        chain.remove_back();
        ++from;
        to = chain.vertices_rbegin();

    }

    // add to back and set pointers
    chain.append(q);
    from = chain.vertices_rbegin();
    ++from;
    to = chain.vertices_rbegin();

    // print the chain after adding
    rLog(graham_yao, "Post-inset:\n%s\tFrom: %s To: %s",
            vect_to_cstring(chain.vertices_begin(), chain.vertices_end()),
            thing_to_cstring(*from), thing_to_cstring(*to));
}


void Convex_hull::Graham_yao::build_upper_hull(Poly_chain_2& chain,
        const Sites& p) {

    std::vector< Point >::const_reverse_iterator iter = p.rbegin();
    ++iter;
    init(chain, *(p.rbegin()), *iter);
    for ( ; iter != p.rend() ; ++iter) {
        add_ccw(chain, *iter);
    }

}


void Convex_hull::Graham_yao::build_lower_hull(Poly_chain_2& chain,
        const Sites& p) {

    std::vector< Point >::const_iterator iter = p.begin(); ++iter;
    init(chain, *(p.begin()), *iter);
    for ( ; iter != p.end() ; ++iter) {
        add_ccw(chain, *iter);
    }

}


//========================================================================
//                              CONVEX HULL
//========================================================================

void Convex_hull::build(const std::vector< Point > & input, int UB) {

    // initialize the class varibales
    U_ = UB;
    sites_ = input;

    // sort the sites
    std::sort(sites_.begin(), sites_.end(), Point_2::precedes);

    // compute the upper and lower hull with graham yao
    Graham_yao gy;
    Poly_chain_2 upper_hull, lower_hull;
    gy.build_upper_hull(upper_hull, sites_);
    gy.build_lower_hull(lower_hull, sites_);

    // splice upper hull and lower hull together storing them
    // into the final convex hull
    ch_.splice(upper_hull);
    ch_.splice(lower_hull);
    ch_.close_chain();
}


std::string Convex_hull::to_string() const {
    std::ostringstream os;
    os << "Sites: " << std::endl;
    os << DDAD_util::vect_to_string(sites_begin(), sites_end());
    os << "Hull: " << std::endl;
    os << polygon();
    return os.str();
}






