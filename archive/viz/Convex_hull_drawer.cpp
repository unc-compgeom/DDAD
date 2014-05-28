/**
 * Filename: Convex_hull_drawer.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 26 May 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "Convex_hull_drawer.h"

void Convex_hull_drawer::draw(const Convex_hull& ch) {

    DDAD_drawer::draw_grid(ch.one(),ch.U());
    DDAD_drawer::draw_polygon(ch.vertices_begin(), ch.vertices_end(),
            DDAD_drawer::Color(1,0,0));
    DDAD_drawer::draw_point_set(ch.sites_begin(), ch.sites_end());

}

