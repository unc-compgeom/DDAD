/**
 * Filename: Constructions_2.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 18 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */


#include "Constructions_2.h"


void Constructions_2::coords(float &x, float &y,
        const Point_2::Std_point& p) {
    x = p.x();
    y = p.y();
}

int Constructions_2::coord_x(const Point_2::Std_point& p) { return p.x(); }

int Constructions_2::coord_y(const Point_2::Std_point& p) { return p.y(); }

float Constructions_2::coord_y_mid_point(const Point_2::Std_point& p,
        const Point_2::Std_point& q) {
    return p.y() + (q.y()-p.y())/2.;
}

float Constructions_2::dist(float px, float py, float qx, float qy) {
    float dx = px-qx;
    float dy = py-qy;
    return std::sqrt(dx*dx+dy*dy);
}



