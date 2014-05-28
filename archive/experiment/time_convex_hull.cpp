/**
 * Filename: time_convex_hull.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 31 May 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include <iostream>
#include <iomanip>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"
#include "../base/Timer.h"

#include "../algos/Convex_hull.h"

int main(int argc, char** argv) {

//    ez_rlog_init(argc,argv);

    int U = (int)pow(2,15); // the grid size
    int N_max = (int)pow(2,15); // number of sites
    Timer t;
    double ch_time;

    for (int N = 1 ; N < N_max ; N *= 2) {

        // generate sites on the UxU grid
        t.start();
        std::cout << "Generating "
            << std::right << std::setw(5) << N << " sites..." << std::flush;
        std::vector< Point_2::Std_point > p;
        DDAD_goodies::create_random_unique(
                std::back_inserter(p), Point_2::Std_point(), N, U);
        t.stop();
        std::cout << std::right << std::setw(5) << t.elapsed_sec()
            << "s, " << std::flush;

        // build the convex hull
        Convex_hull ch;
        std::cout << "Computing Convex Hull...";
        t.start();
        ch.build(p, U);
        ch_time = t.stop();
        std::cout << std::right << std::fixed << std::setw(7)
            << t.elapsed() << "s " << std::flush;
        std::cout.unsetf( std::ios_base::floatfield );
        std::cout << std::endl;
    }
}



