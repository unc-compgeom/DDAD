#define DV


#include <stdio.h>
#include <iostream>

#include <gtest/gtest.h>
#include "../base/DDAD_base.h"

#define TEST_BASE
#define TEST_UTILITIES
#define TEST_PRIMITIVES
#define TEST_PREDICATES
#define TEST_CONSTRUCTIONS


// base
#ifdef TEST_BASE
    #include "test_DDAD_base.cc"
#endif

// utilities
#ifdef TEST_UTILITIES
    #include "test_Radix_sort.cc"
    #include "test_Shuffle.cc"
#endif

// primitives type
#ifdef TEST_PRIMITIVES
    #include "test_Point_2.cc"
    #include "test_Line_2.cc"
    #include "test_Segment_2.cc"
    //#include "test_Quad_edge_ds.cc"
#endif

// more generic low level predicates and operations
#ifdef TEST_PREDICATES
    //#include "test_Numeric_sum.cc"
    #include "test_Predicates_2.cc"
    #include "test_Constructions_2.cc"
#endif

// algorithms and constructions
#ifdef TEST_CONSTRUCTIONS
    #include "test_Convex_hull.cc"
    //#include "test_D2_voronoi.cc"
    #include "test_Discrete_upper_envelope.cc"
    #include "test_NN_transform.cc"
    #include "test_Voronoi_poly_set.cc"
    //#include "test_Gabriel_graph.cc"
    //#include "test_Arrangement.cc"
#endif

int main(int argc, char** argv) {

    // init rlog, write to std error and show errors and warnings.
    ez_rlog_init(argc, argv)

    //======================================================================
    //                 Logs for primitive
    //======================================================================

    /** Numeric summation channels */
    //stdlog.subscribeTo(RLOG_CHANNEL(
    //          "debug/numeric_sum/std_recursive_sum"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/numeric_sum/bin_sum"));

    /** Quadedge channels */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/qeds/test"));

    //======================================================================
    //                 Logs for low level preds and ops
    //======================================================================

    /** radix sort channels */
    //stdlog.subscribeTo( RLOG_CHANNEL( "debug/radix_sort" ));
    //stdlog.subscribeTo( RLOG_CHANNEL( "debug/radix_sort_pair" ));

    //======================================================================
    //                 Logs for algs and construction
    //======================================================================

    /** Convex Hull channels */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/convex_hull"));

    /** Discrete Upper envelope channels */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/brute"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/orient"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/ulogu"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/ric"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/upper_envelope/shared"));

    /** Nearest Neighbor transform */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/nn_transform/init"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/nn_transform/build"));

    /** Arrangment channels */
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/arrangement/add_line"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/arrangement/classify"));


    /** GCD Walk channels */
    //stdlog.subscribeTo( RLOG_CHANNEL( "debug/gcd_walk" ));

    /** D2 Voronoi */
    //stdlog.subscribeTo( RLOG_CHANNEL( "debug/d2_voronoi" ));

       /** Discrete Voronoi */
    //stdlog.subscribeTo(RLOG_CHANNEL(
    //            "debug/nn_transform_base/compute_lex_order"));
    //stdlog.subscribeTo(RLOG_CHANNEL(
    //            "debug/nn_transform_base/compute_envelope"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/discrete_voronoi" ));

    /** Voronoi Polygon Set */
    //stdlog.subscribeTo(RLOG_CHANNEL(
    //            "debug/nn_transform_base/compute_lex_order"));
    //stdlog.subscribeTo(RLOG_CHANNEL(
    //            "debug/nn_transform_base/compute_envelope"));
    //stdlog.subscribeTo(RLOG_CHANNEL("debug/voronoi_poly_set" ));

    std::cout << "Running tests" << std::endl;
    testing::InitGoogleTest(&argc, argv);
    int ret_tests = RUN_ALL_TESTS();

    return ret_tests;
}



