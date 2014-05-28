/**
 * Filename: DDAD_base.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 16 Nov 2010
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "DDAD_base.h"

/**
 * Return the string representation of the sign
 * @param s the sign to convet to s string
 */
std::string DDAD::to_string(const Sign& s) {
    switch (s) {
        case POSITIVE: return "Positive";
        case NEGATIVE: return "Negative";
        case ZERO: return "Zero";
        default: throw(Case_not_covered_error());
    };
}

/**
 * Ouput operator for sign
 * @param s the sgn to output
 */
std::ostream& operator<<(std::ostream& os, const DDAD::Sign& s) {
    return os << DDAD::to_string(s);
}

