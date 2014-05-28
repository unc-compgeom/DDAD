/**
 * Filename: nn_transform.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 11 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include <iostream>
#include <fstream>
#include <iterator>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"
#include "../base/Timer.h"

#include "../algos/NN_transform.h"

#include "../viz/DDAD_drawer.h"
#include "../viz/NN_transform_drawer.h"
#include "../viz/OGL.h"

typedef Point_2::Std_point Point;

NN_transform nn;
DDAD_drawer::Color_map map;
bool verify_on = true;

void user_keyboard_press(unsigned char key, int x, int y) {
    if (key == 'j') { DDAD_drawer::refill_with_random_colors(map); }
    if (key == ';') { ox_gg.world_rotate_z += 90; }
}
void project_specifics() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ox_gg.world_scale = 15./nn.U();
}

void draw_objects() {
    glTranslatef(-nn.U()/2.,-nn.U()/2.,0);
    NN_transform_drawer::draw(nn, map);
}


void read_pts(std::vector<Point>& S, int& U, std::istream& in) {
    U = -1;
    int pid = 0;
    std::string line;
    while ( in.good() ) {
        getline(in, line);
        std::stringstream ss(line);
        std::istream_iterator<std::string> ii(ss);

        if (*ii == "U") {
            ++ii; // advance pased "U"
            U = atoi(ii->c_str()); ++ii;
        } else if (*ii == "p") {
            ++ii; // advance pased "p"
            int x = atoi(ii->c_str()); ++ii;
            int y = atoi(ii->c_str()); ++ii;
            S.push_back(Point(x,y,pid));
            ++pid;
        }
    }
}



int main(int argc, char** argv) {

    int U=1;
    std::vector< Point > S;


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

    // verify correct input format
    char* file_name;
    if (argc < 2) {
        std::cerr << argv[0] << " <inFileName>";
        exit(1);
    } else {
        file_name = argv[1];
    }

    std::ifstream in(file_name);
    if (in.fail()) {
        std::cerr << "Couldn't open file " << file_name << std::endl;
        exit(1);
    }

    // open and read file
    read_pts(S,U,in);
    rAssert(S.size() > 1 && U>0);

    // output points and U
    std::cout << "Points:" << std::endl;
    //std::copy(S.begin(), S.end(), std::ostream_iterator<Point>(std::cout,"\n"));
    std::cout << "U: " << U << std::endl;

    std::cout << "Sorting sites: " << std::endl;
    NN_transform::sort_sites(S,U);

    // build and output nearest neighbor transform
    std::cout << "Computing the nearest neighbor transform: " << std::endl;
    Timer t;
    t.start();
    //nn.build_deg_2_usq(S, U, true);
    //nn.build_deg_2_usq_logu(S, U, true);
    nn.build_deg_3_usq(S, U, true);
    t.stop();
    std::cout << "Time: " << t.elapsed() << std::endl;
    //std::cout << nn << std::endl;

    // verify
    if (verify_on) {
        std::cout << "Verifying the nearest neighbor transform:..." << std::flush;
        if (nn.verify(S, U)) { std::cout << "success" << std::endl; }
        else { std::cout << "failure" << std::endl; }
    } else {
        std::cout << "Verification off." << std::endl;
    }

    // fill the color map
    DDAD_drawer::fill_with_random_colors(map, S.size());

    // draw
    setupOGL(0, NULL);

    return 0;
}



