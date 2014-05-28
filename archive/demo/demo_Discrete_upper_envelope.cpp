/**
 * Filename: demo_Discrete_upper_envelope.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 02 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */


#include <iterator>

#include "../base/DDAD_base.h"
#include "../base/Line_2.h"

#include "../algos/Discrete_upper_envelope.h"

#include "../viz/DDAD_drawer.h"
#include "../viz/Discrete_upper_envelope_drawer.h"
#include "../viz/OGL.h"


typedef Line_2::Slope_and_y_intercept Line;

DDAD_drawer::Color_map map;
Discrete_upper_envelope due;
std::vector< Line > L;
bool draw_all_lines = true;

void user_keyboard_press(unsigned char key, int x, int y) {
    if (key == 'j') { DDAD_drawer::refill_with_random_colors(map); }
    if (key == 'k') { draw_all_lines = !draw_all_lines; }

}
void project_specifics() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void draw_objects() {
    glTranslatef(-5,-5,0);

    if (draw_all_lines) {
        DDAD_drawer::draw_line_set(L.begin(), L.end(), due.one(), due.U(), map);
    }
    Discrete_upper_envelope_drawer::draw(due, map);
}

int main(int argc, char** argv) {
    int U = 100; // the grid size
    int M = 5; // the grid size
    int B = 20; // the grid size
    int N = 20; // the number of sites

    // init rlog, write to std error and show errors and warnings.
    ez_rlog_init(argc, argv)

    /** Discrete Upper envelope channels */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/brute"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/orient"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/ulogu"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/ric"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/shared"));


    DDAD_goodies::seed_rand();

    // generate N lines
    std::cout << "Generating lines:\n";
    DDAD_goodies::create_random_unique(
            std::back_inserter(L), Line(), N, M, B, false, false);

    // outptu the lines
    std::copy(L.begin(), L.end(),
            std::ostream_iterator< Line >(std::cout,"\n"));
    std::cout << std::endl;

    // init the color map
    DDAD_drawer::fill_with_random_colors(map, N);

    // build and output the DUE of L using all algs
    std::cout << "Computing the discrete upper envelope:" << std::endl;
    DDAD_goodies::seed_rand();
    due.build_ric(L, -U, U);
    std::cout << due << std::endl;

    // verify
    std::cout << "Verifying the discrete upper envelope:..." << std::flush;
    Discrete_upper_envelope due_ulogu, due_orient;
    due_ulogu.build_ulogu(L, -U, U);
    due_orient.build_orient(L, -U, U);
    assert(due.verify(L));
    assert(due_ulogu.verify(L));
    assert(due_orient.verify(L));
    std::cout << "success" << std::endl;


    // draw
    setupOGL(0, NULL);
}



