/**
 * Filename: DDAD_drawer.h
 * Project name: degDrivenAlgoDesign
 * Created on: 08 Dec 2010
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_DDAD_DRAWER_H_
#define _DDAD_DDAD_DRAWER_H_

// apple specific openGL includes
#ifdef __APPLE__
    #include "GLUT/glut.h"
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif

#include "../base/Constructions_2.h"

struct DDAD_drawer {

    /** return a random color */
    static float my_rand() { return (double)rand()/(double)RAND_MAX; }

public:

    struct Color {
        float r, g, b;
        Color(float ri=0, float gi=0, float bi=0) : r(ri), g(gi), b(bi) {}
    };

    typedef std::vector< Color > Color_map;

    static Color get_color(const Color_map& map, int id) {
        return (0 <= id && id < (int)map.size()) ? map[id] : Color(1,1,1);
    }

    static float rand_color() { return .2 + .8*my_rand(); }

    /** fill m with a random color map of N entries */
    static void fill_with_random_colors(Color_map& m, int N) {
        rAssert(N > 0);
        if (m.size() > 0) { m.clear(); }
        for (int i = 0 ; i < N ; ++i) {
            m.push_back( Color(rand_color(), rand_color(), rand_color()) );
        }
    }

    /** fill m with a random color map of N entries */
    static void refill_with_random_colors(Color_map& m) {
        fill_with_random_colors(m, m.size());
    }



    static void draw_grid(int one, int U) {
        glPointSize(1);
        glBegin(GL_POINTS);
        glColor3f(1,1,1);
        for (int x = one ; x <= U ; ++x) {
            for (int y = one ; y <= U ; ++y) { glVertex2f(x, y); }
        }
        glEnd();
    }

    template <class InputIter>
    static void draw_polygon(InputIter begin, InputIter end,
            Color c=Color(1,1,1)) {
        float x,y;
        glBegin(GL_LINE_STRIP);
        glColor3f(c.r,c.g,c.b);
        for (InputIter j = begin ; j != end ; ++j) {
            Constructions_2::coords(x,y,*j);
            glVertex2f(x, y);
        }
        Constructions_2::coords(x,y,*begin);
        glVertex2f(x, y);
        glEnd();
    }

    template <class InputIter>
    static void draw_point_set(InputIter begin, InputIter end) {
        float x,y;
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(1,1,1);
        for (InputIter j = begin ; j != end ; ++j) {
            Constructions_2::coords(x,y,*j);
            glVertex2f(x, y);
        }
        glEnd();
    }

    template <class InputIter>
    static void draw_line_set(InputIter begin, InputIter end, int left, int right,
            const Color_map& map) {
        float xl,yl,xr,yr;
        Line_2::Vertical left_bdd(left), right_bdd(right);
        glBegin(GL_LINES);
        for (InputIter j = begin ; j != end ; ++j) {
            Color color = DDAD_drawer::get_color(map, j->id());
            glColor3f(color.r,color.g,color.b);
            Line_2::intersection_coords(xl,yl,*j,left_bdd);
            Line_2::intersection_coords(xr,yr,*j,right_bdd);
            glVertex2f(xl, yl);
            glVertex2f(xr, yr);
        }
        glEnd();
    }

};

#endif

