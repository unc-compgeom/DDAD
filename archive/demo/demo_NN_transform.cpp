/**
 * Filename: demo_NN_transform.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 07 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include <iterator>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"
#include "../base/Timer.h"

#include "../algos/NN_transform.h"

#include "../viz/DDAD_drawer.h"
#include "../viz/NN_transform_drawer.h"
#include "../viz/OGL.h"


NN_transform nn;
DDAD_drawer::Color_map map;

void user_keyboard_press(unsigned char key, int x, int y) {
    if (key == 'j') { DDAD_drawer::refill_with_random_colors(map); }
}
void project_specifics() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void draw_objects() {
    glTranslatef(-5,-5,0);
    NN_transform_drawer::draw(nn, map);
}

int main(int argc, char** argv) {

    typedef Point_2::Std_point Point;

    int U = 10; // the number of sites
    int N = 10; // the grid size

    // init rlog, write to std error and show errors and warnings.
    ez_rlog_init(argc, argv);

    /** Discrete Upper envelope channels */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/brute"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/orient"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/ulogu"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/ric"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/shared"));

    /** Nearest Neighbor transform */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/nn_transform/init"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/nn_transform/build"));

    // generate N sites on the UxU grid
    std::cout << "Generating sites:\n";
    std::vector< Point > S;
    DDAD_goodies::seed_rand();
    DDAD_goodies::create_random_unique(std::back_inserter(S), Point(), N, U);
    std::copy(S.begin(), S.end(), std::ostream_iterator<Point>(std::cout,"\n"));
    std::cout << std::endl;

    // build and output nearest neighbor transform
    std::cout << "Computing the nearest neighbor transform: " << std::endl;
    nn.build(S, U);
    std::cout << nn << std::endl;

    // verify
    std::cout << "Verifying the nearest neighbor transform:..." << std::flush;
    // run other algs on this data set
    if (!nn.verify(S,U)) { std::cout << "success" << std::endl; }
    else { std::cout << "fail" << std::endl; }

    // fill the color map
    DDAD_drawer::fill_with_random_colors(map, N);

    // draw
    setupOGL(0, NULL);
}



