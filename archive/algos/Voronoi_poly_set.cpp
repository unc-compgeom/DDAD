/**
 * Filename: Voronoi_poly_set.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 14 Aug 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 *         Steven Love <slove13@cs.unc.edu>
 */

#include "Voronoi_poly_set.h"


void Voronoi_poly_set::unpack(const Discrete_upper_envelope& due, int y) {
    std::cout << "Unpacking DUE along " << y << std::endl;
}


void Voronoi_poly_set::allocate(int one, int U) {
    std::cout << "allocate space for the Voronoi polygon set's data structures\n";
}


std::string Voronoi_poly_set::to_string() const {
    std::stringstream os;
    os << "This is my string representation" << std::endl;
    return os.str();
}

