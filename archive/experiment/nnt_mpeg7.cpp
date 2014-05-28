/** * Filename: nnt_size_and_density.cpp * Project name: degDrivenAlgoDesign
 * Created on: 15 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */


#include <sstream>
#include <iomanip>
#include <fstream>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"
#include "../base/Timer.h"

#include "../algos/NN_transform.h"

typedef Point_2::Std_point Point;

template < typename T0, typename T1, typename T3, typename T4, typename T5, typename T6>
void to_os(std::ostringstream& os, const T0& algo,
        const T1& grid_size, const T3& num_sites,
        const T4& sort_time, const T5& num_iter_for_nn_time,
        const T6& nnt_time) {
    os  << std::left <<  std::setw(15) << algo
        << std::left <<  std::setw(15) << grid_size
        << std::left << std::setw(15) << num_sites
        << std::left << std::setw(15) << sort_time
        << std::left << std::setw(15) << num_iter_for_nn_time
        << std::left << std::setw(15) << std::setprecision(5) << nnt_time << std::endl;
}

template < typename T1 >
void comment(std::ostringstream& os, const T1& comment) {
    os << "% " << comment << std::endl;
}

template < typename T1, typename T2 >
void comment(std::ostringstream& os, const T1& comment, const T2& comment2) {
    os << "% " << comment << " " << comment2 << std::endl;
}


template < typename T1, typename T2, typename T3, typename T4 >
void comment(std::ostringstream& os, const T1& comment, const T2& comment2,
        const T3& comment3, const T4& comment4) {
    os << "% " << comment << " " << comment2 << " "
        << comment3 << " " << comment4 << std::endl;
}

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
void comment(std::ostringstream& os, const T1& comment, const T2& comment2,
        const T3& comment3, const T4& comment4, const T5& comment5) {
    os << "% " << comment << " " << comment2 << " " << comment3 << " "
        << comment4 << " " << comment5 << std::endl;
}

template < typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6 >
void comment(std::ostringstream& os, const T1& comment, const T2& comment2,
        const T3& comment3, const T4& comment4,
        const T5& comment5, const T6& comment6) {
    os << "% " << comment << " " << comment2 << " " << comment3 << " "
        << comment4 << " " << comment5 << " " <<  comment6 << std::endl;
}

template < typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6, typename T7 >
void comment(std::ostringstream& os, const T1& comment, const T2& comment2,
        const T3& comment3, const T4& comment4,
        const T5& comment5, const T6& comment6,
        const T7& comment7) {
    os << "% " << comment << " " << comment2 << " " << comment3 << " "
        << comment4 << " " << comment5 << " " <<  comment6 << " "
        << comment7 << std::endl;
}



template < typename T0, typename T1, typename T3, typename T4, typename T5, typename T6>
void comment_line(std::ostringstream& os, const T0& algo, const T1& grid_size, const T3& num_sites,
        const T4& sort_time, const T5& num_iter_for_nn_time, const T6& nnt_time) {
    std::ostringstream a;
    a << "% " << algo;
    to_os(os, a.str(), grid_size, num_sites, sort_time, num_iter_for_nn_time, nnt_time);
}

int algo_to_num(const std::string& a) {
    if (a == "U2") { return 0; }
    else if (a == "U2LogU") { return 1; }
    else if (a == "Deg3") { return 2; }
    else if (a == "MATLAB") { return 3; }
    else { return -1; }
}

struct U2 {
    int seed;
    U2(int seed_in) : seed(seed_in) { }
    std::string name() const { return "U2"; }
    template < typename Points >
    double run(NN_transform& nn, const Points& p, int U,
            bool sorted, int num_iter) const {
        Timer t;
        t.start();
        for (int k = 0 ; k < num_iter ; ++k) {
            DDAD_goodies::seed_rand(seed);
            nn.build_deg_2_usq(p, U, sorted);
        }
        double time = t.stop_millisec();
        // now remove seeding time
        t.start();
        for (int k = 0 ; k < num_iter ; ++k) {
            DDAD_goodies::seed_rand(seed);
        }
        time -= t.stop_millisec();
        return time;
    }
};

struct U2LogU {
    std::string name() const { return "U2LogU"; }
    template < typename Points >
    double run(NN_transform& nn, const Points& p, int U,
            bool sorted, int num_iter) const {
        Timer t;
        t.start();
        for (int k = 0 ; k < num_iter ; ++k) {
            nn.build_deg_2_usq_logu(p, U, sorted);
        }
        return t.stop_millisec();
    }
};

struct Deg3 {
    std::string name() const { return "Deg3"; }
    template < typename Points >
    double run(NN_transform& nn, const Points& p, int U,
            bool sorted, int num_iter) const {
        Timer t;
        t.start();
        for (int k = 0 ; k < num_iter ; ++k) {
            nn.build_deg_3_usq(p, U, sorted);
        }
        return t.stop_millisec();
    }
};


template < typename Algo , typename Points >
void run_experiment(double& nnt_time, int& num_iter, const Algo& algo,
        const Points& p, int U, bool sorted, double min_time) {
    std::cerr << algo.name() << "(" << std::flush;
    NN_transform nn;
    nnt_time = 0;
    float ni = .5;
    if (min_time <= 0) {
        num_iter = 1;
        std::cerr << num_iter << ")..." << std::flush;
        nnt_time = algo.run(nn, p, U, sorted, num_iter);
    } else {
        // if min_time is 0 just run once
        if (min_time == 0) { nnt_time = algo.run(nn, p, U, sorted, 1); }
        // use min timings
        else {
            while (nnt_time < 100) { // at least get a sample that is 100 ms
                ni *= 2;
                nnt_time = algo.run(nn, p, U, sorted, (int)ni);
            }
            num_iter = ((int)ni)*std::ceil( min_time / nnt_time);
            std::cerr << num_iter << ")..." << std::flush;

            // if 1 iter is the answer, were don't need to do anything
            if (num_iter != 1) {
                nnt_time = algo.run(nn, p, U, sorted, (int)num_iter);
            }
        }
    }
    std::cerr << nnt_time << "ms " << std::flush;
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

    ez_rlog_init(argc,argv);

    const int ALGO = 1;
    const int PATH = 2;
    const int PREFIX = 3;
    const int LAST_FILE = 4;
    const int NUM_EXPER = 5;
    const int MIN_TIME = 6;
    const int NUM_ARGS = 7;

    if (argc != NUM_ARGS) {
        std::cerr << "Invalid arguments" << std::endl;
        std::cerr << argv[0] << " <algo> <path> <prefix> <last_num> <num_exper> <min_time_in_ms>\n"
            << std::endl
            << "Where:\n"
            << "        <algo> - must be U2, U2logU, Deg3, or All - Defines the algorithm(s) to run.\n"
            << "        <path> is the path to the mpeg7 files set to run.\n"
            << "        <mpeg7_prefix> is the prefix for the mpeg7 files set to run.\n"
            << "        <last_num> is the last number to run (inclusive).\n"
            << "        <num_exper> - must be an integer - defines the number of experiments to run per image.\n"
            << "For smaller examples, we may need to average multiple iterations of an experiment to get enough clock resolution.\n"
            << "        <min_time_in_ms> - must be non-neg- defines the shortest acceptable time for an expirement in milliseconds.\n"
            << std::endl
            << "For example: \n"
            << "        > " << argv[0] << " All ../../data/mpeg7/turtle/ turtle- 4 5 1000\n"
            << std::endl
            << "Runs a job using all algorithms on turtle-1.gif.bdd.pts to turtle-4.gif.bdd.pts\n"
            << "and runs each image 5 times.  Resulting in 20 data points.  For each image \n"
            << "we count how many iterations are needed to reach .1 seconds and determine the number of iterations i\n"
            << "needed to take a 1s (1000ms).  We then run the experiment i times.\n";
        return 0;
    }

    // parse the input
    std::string algo(argv[ALGO]);
    std::string file_path(argv[PATH]);
    std::string file_prefix(argv[PREFIX]);
    int last_file = std::atoi(argv[LAST_FILE]);
    int num_exper = std::atoi(argv[NUM_EXPER]);
    float min_time = std::atof(argv[MIN_TIME]);

    Timer t;
    int seed = 250385; // seed with Bee's BD
    DDAD_goodies::seed_rand(seed);

    std::cerr << "----------------Variables----------------\n"
        << "Seed: " << seed << std::endl
        << "Algo: " << algo << std::endl
        << "Path: " << file_path << std::endl
        << "Prefix: " << file_prefix << " [1:" << last_file << "]" << std::endl
        << "numExper: " << num_exper << std::endl
        << "minTime: " << min_time << std::endl
        << "-----------------------------------------\n";

    std::ostringstream os;
    comment(os, "This experiment produces timing for computing the NNTransform of an image");
    comment(os, "using various algorithms, on images from the mpeg7 data set.");
    comment(os, "Algos are mapped to numbers");
    comment(os, "\tU2 <-> ", algo_to_num("U2"));
    comment(os, "\tU2LogU <-> ", algo_to_num("U2LogU"));
    comment(os, "\tDeg3 <-> ", algo_to_num("Deg3"));
    comment(os, "\tMATLAB <-> ", algo_to_num("Deg3"));
    comment(os, "Seed:", seed);
    comment(os, "Algo: ", algo);
    comment(os, "Path: ", file_path);
    comment(os, "Prefix: ", file_prefix, " [1:", last_file, "]");
    comment(os, "numExper:", num_exper);
    comment(os, "minTime: ", min_time);
    comment(os, "NumExper:", num_exper);
    comment_line(os, "algo", "grid_size", "num_sites", "sort_time", "nn_time_iter", "nnt_time");

    for (int iimg = 1 ; iimg <= last_file ; ++iimg) {

        // read pixels
        int U;
        std::vector< Point > S;

        // config
        bool sorted = true;

        std::stringstream in_fname_build;
        in_fname_build << file_path << "/" << file_prefix << "/" << file_prefix << "-" << iimg << ".gif.bdd.pts";
        std::string in_file_name = in_fname_build.str();
        std::ifstream in(in_file_name.c_str());
        if (in.fail()) {
            std::cerr << "Couldn't open file " << in_file_name << std::endl;
            exit(1);
        }

        // open and read file
        read_pts(S,U,in);
        rAssert(S.size() > 1 && U>0);

        for (int i = 0 ; i < num_exper ; ++i) {
            // output file info
            std::cerr << "(" << U << ")\t"
                << "(" << iimg  << "/" << last_file << ")"
                << "#Sites: " << S.size() << "..." << std::flush;

            // sort the sites
            double sort_time = 0;
            if (sorted) {
                std::cerr << "Sorting..." << std::flush;
                t.start();
                NN_transform::sort_sites(S, U);
                sort_time = t.stop_millisec();
                std::cerr << sort_time << "ms " << std::flush;
            }

            // build the nn transform using U_SQ
            double nnt_time = -1;
            int num_iter = -1;
            if (algo == "All" || algo == "U2") {
                run_experiment(nnt_time, num_iter, U2(seed), S, U, sorted, min_time);
                to_os(os, algo_to_num("U2"), U, S.size(), sort_time, num_iter, nnt_time);
            }

            if (algo == "All" || algo == "U2LogU") {
                run_experiment(nnt_time, num_iter, U2LogU(), S, U, sorted, min_time);
                to_os(os, algo_to_num("U2LogU"), U, S.size(), sort_time, num_iter, nnt_time);
            }

            if (algo == "All" || algo == "Deg3") {
                run_experiment(nnt_time, num_iter, Deg3(), S, U, sorted, min_time);
                to_os(os, algo_to_num("Deg3"), U, S.size(), sort_time, num_iter, nnt_time);
            }
            std::cerr << std::endl;
        }
    }
    std::cout << os.str() << std::endl;

    // build the file name
    std::ostringstream fname;
    fname << "timings_for_Algo:" << algo << "_prefix:" << file_prefix
        << "_numExper:" << num_exper << "_minTime:" << min_time;
    std::cout << "File name: " << fname.str() << std::endl;
    std::ofstream file;
    file.open(fname.str().c_str());
    file << os.str() << std::endl;
    file.close();
    return 0;
}



