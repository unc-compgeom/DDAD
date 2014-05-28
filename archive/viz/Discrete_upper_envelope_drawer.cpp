/**
 * Filename: Discrete_upper_envelope_drawer.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 02 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "Discrete_upper_envelope_drawer.h"


void Discrete_upper_envelope_drawer::draw(const Discrete_upper_envelope& due,
        const DDAD_drawer::Color_map& map) {

    typedef Discrete_upper_envelope::const_iterator Due_iter;

    // draw the grid
    DDAD_drawer::draw_grid(due.one(), due.U());

    // draw the lines of the DUE
    Line_2::Vertical left_bdd(due.one()), right_bdd(due.U());
    float xl,yl,xr,yr;
    glBegin(GL_LINES);
    for (Due_iter cell = due.begin() ; cell != due.end() ; ++cell) {

        const DDAD_drawer::Color& color = DDAD_drawer::get_color(map, cell->id());
        Line_2::intersection_coords(xl,yl,cell->line,left_bdd);
        Line_2::intersection_coords(xr,yr,cell->line,right_bdd);

        glColor3f(color.r,color.g,color.b);
        glVertex2f(xl, yl);
        glVertex2f(xr, yr);
    }
    glEnd();

    // draw the points of the due at an interval
    const int y_interval = 10;
    glPointSize(5);
    glBegin(GL_POINTS);
    for (Due_iter cell = due.begin() ; cell != due.end() ; ++cell) {
        const DDAD_drawer::Color& color = DDAD_drawer::get_color(map, cell->id());
        glColor3f(color.r,color.g,color.b);

        for (int i = cell->left ; i <= cell->right ; ++i) {
            for (int j = due.one() ; j <= due.U(); j+=y_interval) {
                glVertex2f(i,j);
            }
        }
    }
    glEnd();
}

