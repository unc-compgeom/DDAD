/**
 * Filename: demo_Convex_hull.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 26 May 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include <iterator>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"

#include "../algos/Convex_hull.h"

#include "../viz/DDAD_drawer.h"
#include "../viz/Convex_hull_drawer.h"
#include "../viz/OGL.h"

Convex_hull ch;
std::vector< Point_2::Std_point > p;

void user_keyboard_press(unsigned char key, int x, int y) { }
void project_specifics() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void draw_objects() {
    glTranslatef(-5,-5,0);
    Convex_hull_drawer::draw(ch);
}

int main(int argc, char** argv) {
    int U = 10; // the grid size
    int N = 25; // the number of sites


    // generate N sites on the UxU grid
    std::cout << "Generating points:\n";
    DDAD_goodies::create_random_unique(std::back_inserter(p),
            Point_2::Std_point(), N, U);

    //Point_2 p0(1,4,0); // elt of lower and upper hull
    //Point_2 p1(3,2,1); // elt of lower hull
    //Point_2 p2(4,6,2); // elt of upper hull
    //Point_2 p3(5,4,3); //
    //Point_2 p4(5,6,4); //
    //Point_2 p5(6,7,5); // elt of upper hull
    //Point_2 p6(7,5,6); //
    //Point_2 p7(8,1,7); // elt of lower hull
    //Point_2 p8(10,6,8); // elt of lower and upper hull
    //p.push_back(p0);
    //p.push_back(p1);
    //p.push_back(p2);
    //p.push_back(p3);
    //p.push_back(p4);
    //p.push_back(p5);
    //p.push_back(p6);
    //p.push_back(p7);
    //p.push_back(p8);


    std::copy(p.begin(), p.end(),
            std::ostream_iterator<Point_2::Std_point>(std::cout,"\n"));
    std::cout << std::endl;

    // build and output the convex hull of p
    std::cout << "Computing the convex hull:" << std::endl;
    ch.build(p, U);
    std::cout << ch << std::endl;

    // draw
    setupOGL(0, NULL);
}



