/**
 * Filename: time_nn_transform.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 12 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */




#include <iostream>
#include <iomanip>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"
#include "../base/Timer.h"

#include "../algos/NN_transform.h"

int main(int argc, char** argv) {

    ez_rlog_init(argc,argv);

    /** Discrete Upper envelope channels */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/brute"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/orient"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/ulogu"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/ric"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/shared"));

    /** Nearest Neighbor transform */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/nn_transform/init"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/nn_transform/build"));

    if (argc != 1 && argc != 4) {
        std::cerr << "Error args:\n"
            << argv[0] << " <uBase> <density> <onlyU2>\n"
            << "  with density in (0,1), onlyU2 as {0,1}\n";
        return 1;
    }

    int U_base_min = (argc == 1) ?  7 : atoi(argv[1]);
    int U_base_max = (argc == 1) ? 12 : U_base_min;
    float density =  (argc == 1) ? .1 : atof(argv[2]);
    bool U2_only =   (argc == 1) ?  0 : atoi(argv[3]);
    std::ostringstream os;
    for (int U_base = U_base_min ; U_base <= U_base_max ; ++U_base) {

        int U = std::pow(2.,U_base);
        bool sorted = true;
        float percent = density;
        // generate sites
        Timer t;
        t.start();
        std::cout << "(" << U << ")\t"
            << "Density " << percent  << "..." << std::flush;
        os << "(" << U << ")\t" << "Density " << percent << "...";
        std::vector< Point_2::Std_point > p;
        DDAD_goodies::create_random_unique_percent(
                std::back_inserter(p), Point_2::Std_point(), percent, U);

        rAssert(p.size() > 0);
        t.stop();
        std::cout << t.elapsed_sec() << "s "
            << std::right << std::setw(10) << p.size() << " sites, " << std::flush;
        os << p.size() << " sites, ";

        // sort the sites
        if (sorted) {
            std::cout << "Sorting..." << std::flush;
            t.start();
            NN_transform::sort_sites(p, U);
            t.stop();
            std::cout <<  t.elapsed_sec() << "s, " << std::flush;
        }

        // build the nn transform
        NN_transform nn;
        std::cout << "Computing NN Trans:" << std::flush;

        std::cout << " RIC..." << std::flush;
        t.start();
        nn.build_deg_2_usq(p, U, sorted);
        t.stop();
        std::cout << std::right << std::fixed << std::setw(10)
            << t.elapsed() << "s " << std::flush;
        std::cout.unsetf( std::ios_base::floatfield );
        os << "RIC: " << t.elapsed() << "s ";

        if (!U2_only) {
            std::cout << " UlogU..." << std::flush;
            t.start();
            nn.build_deg_2_usq_logu(p, U, sorted);
            t.stop();
            std::cout << std::right << std::fixed << std::setw(10)
                << t.elapsed() << "s " << std::flush;
            std::cout.unsetf( std::ios_base::floatfield );
            os << "UlogU: " << t.elapsed() << "s ";

            std::cout << " Deg3..." << std::flush;
            t.start();
            nn.build_deg_3_usq(p, U, sorted);
            t.stop();
            std::cout << std::right << std::fixed << std::setw(10)
                << t.elapsed() << "s " << std::flush;
            std::cout.unsetf( std::ios_base::floatfield );
            os << "Deg3: " << t.elapsed() << "s ";
        }
        std::cout << std::endl;

        // clear this run if it is not the last
        if (U_base < U_base_max) { os.str(""); }
    }
    std::cout << os.str() << std::endl;
}



