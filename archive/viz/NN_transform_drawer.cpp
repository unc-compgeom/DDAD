/**
 * Filename: NN_transform_drawer.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 07 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "NN_transform_drawer.h"

const int POINT_SIZE = 5;


void NN_transform_drawer::draw_grid(const NN_transform& nn,
        const Color_map& color_map) {

    Color c;
    glPointSize(4*POINT_SIZE);
    glBegin(GL_POINTS);
    for (int x = nn.one() ; x <= nn.U() ; ++x) {
        for (int y = nn.one() ; y <= nn.U() ; ++y) {
            c = DDAD_drawer::get_color(color_map, nn.post_office_query(x,y));
            glColor3f(c.r, c.g, c.b);
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void NN_transform_drawer::draw_sites(const NN_transform& nn,
        const Color_map& color_map) {
    float x,y;
    glPointSize(2*POINT_SIZE);
    glBegin(GL_POINTS);
    for (Site_iterator i = nn.site_begin() ; i != nn.site_end() ; ++i) {
        glColor3f(0,0,0);
        Constructions_2::coords(x,y, *i);
        glVertex2f(x,y);
    }
    glEnd();

    glPointSize(POINT_SIZE);
    glBegin(GL_POINTS);
    // draw the sites
    for (Site_iterator i = nn.site_begin() ; i != nn.site_end() ; ++i) {
        glColor3f(1,1,1);
        Constructions_2::coords(x,y, *i);
        glVertex2f(x, y);
    }
    glEnd();
}


void NN_transform_drawer::draw(const NN_transform& nn, const Color_map& color_map) {
    // draw the grid
    draw_grid(nn, color_map);

    // draw the sites
    draw_sites(nn, color_map);
}

