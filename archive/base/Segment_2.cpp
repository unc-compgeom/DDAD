/**
 * Filename: Segment_2.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 08 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "Segment_2.h"


std::string Segment_2::Std_segment::to_string() const {
    std::ostringstream os;
    if (id() >= 0) { os << id() << ": "; }
    os << "[ " << p1() << " -- " << p2() << " ]";
    return os.str();
}


