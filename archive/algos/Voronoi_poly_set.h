/**
 * Filename: Voronoi_poly_set.h
 * Project name: oxLibCSG
 * Created on: 14 Aug 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 *         Steven Love <slove13@cs.unc.edu>
 */

#ifndef _VORONOI_POLY_SET_H_
#define _VORONOI_POLY_SET_H_

#include <string>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"
#include "../base/Predicates_2.h"
#include "../base/Constructions_2.h"

#include "../algos/Discrete_upper_envelope.h"
#include "../algos/NN_transform.h"

class Voronoi_poly_set : public NN_transform {

public:
    /** Unpack the due into the Voronoi polygon set along line Y=y */
    virtual void unpack(const Discrete_upper_envelope& due, int y);

    /** allocate memory for the voronoy polygons set computed over region one...U */
    virtual void allocate(int one, int U);

    /** return the strung representation of the discrete voronoi diagram */
    virtual std::string to_string() const;

    /** unsupported for VPS */
    void build_brute_force(const std::vector< Point >& input, int UB) {
        rError("Operation not supported for Voronoi Polygon Set");
    }

    /** unsupported for VPS */
    bool verify(const std::vector< Point >& input, int UB) const {
        rError("Operation not supported for Voronoi Polygon Set");
        return 0;
    }

    /** unsupported for VPS */
    int post_office_query(int x, int y) const {
        rError("Operation not supported for Voronoi Polygon Set");
        return 0;
    }

    /** unsupported for VPS */
    float dist_query(int x, int y) const {
        rError("Operation not supported for Voronoi Polygon Set");
        return 0;
    }
};

#endif
