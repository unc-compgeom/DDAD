/**
 * Filename: test_Convex_hull.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 22 Feb 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "../base/Point_2.h"

#include "../algos/Convex_hull.h"

#include <gtest/gtest.h>


TEST(ConvexHullGrahamYao, initLeftToRight) {
    Point_2::Std_point p0(1,4,0); // elt of lower and upper hull
    Point_2::Std_point p1(3,2,1); // elt of lower hull

    // test init sweeping left to right
    Convex_hull::Graham_yao ch_left;
    Poly_chain_2 chain;
    ch_left.init(chain, p0, p1);
    EXPECT_EQ(p0, *(ch_left.from));
    EXPECT_EQ(p1, *(ch_left.to));
}


TEST(ConvexHullGrahamYao, initTopToBottom) {
    Point_2::Std_point p1(3,2,1); // elt of lower hull
    Point_2::Std_point p7(8,1,7); // elt of lower hull

    // test init sweeping bottom to top
    Convex_hull::Graham_yao ch_bottom;
    Poly_chain_2 chain;
    ch_bottom.init(chain, p7, p1);
    EXPECT_EQ(p7, *(ch_bottom.from));
    EXPECT_EQ(p1, *(ch_bottom.to));
}


TEST(ConvexHullGrahamYao, addCWLeftToRight) {
    Point_2::Std_point p0(1,4,0); // elt of lower and upper hull
    Point_2::Std_point p1(3,2,1); // elt of lower hull
    Point_2::Std_point p2(4,6,2); // elt of upper hull
    Point_2::Std_point p3(5,4,3); //
    Point_2::Std_point p4(5,6,4); //
    Point_2::Std_point p5(6,7,5); // elt of upper hull
    Point_2::Std_point p6(7,5,6); //
    Point_2::Std_point p7(8,1,7); // elt of lower hull
    Point_2::Std_point p8(10,6,8); // elt of lower and upper hull

    // test computing upper hull left to right
    Convex_hull::Graham_yao ch;
    Poly_chain_2 chain;
    ch.init(chain, p0, p1);
    EXPECT_EQ(p0, *(ch.from));
    EXPECT_EQ(p1, *(ch.to));

    // add p2
    ch.add_cw(chain, p2);
    EXPECT_EQ(2, chain.num_vertices());
    Poly_chain_2::Vertex_list::const_iterator iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p2, *iter); ++iter;
    EXPECT_TRUE(chain.vertices_end() == iter);
    EXPECT_EQ(p0, *(ch.from));
    EXPECT_EQ(p2, *(ch.to));

    // add p3
    ch.add_cw(chain, p3);
    EXPECT_EQ(3, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p2, *iter); ++iter;
    EXPECT_EQ(p3, *iter); ++iter;
    EXPECT_EQ(p2, *(ch.from));
    EXPECT_EQ(p3, *(ch.to));

    // add p4
    ch.add_cw(chain, p4);
    EXPECT_EQ(3, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p2, *iter); ++iter;
    EXPECT_EQ(p4, *iter); ++iter;
    EXPECT_EQ(p2, *(ch.from));
    EXPECT_EQ(p4, *(ch.to));

    // add p5
    ch.add_cw(chain, p5);
    EXPECT_EQ(3, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p2, *iter); ++iter;
    EXPECT_EQ(p5, *iter); ++iter;
    EXPECT_EQ(p2, *(ch.from));
    EXPECT_EQ(p5, *(ch.to));

    // add p6
    ch.add_cw(chain, p6);
    EXPECT_EQ(4, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p2, *iter); ++iter;
    EXPECT_EQ(p5, *iter); ++iter;
    EXPECT_EQ(p6, *iter); ++iter;
    EXPECT_EQ(p5, *(ch.from));
    EXPECT_EQ(p6, *(ch.to));

    // add p7
    ch.add_cw(chain, p7);
    EXPECT_EQ(5, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p2, *iter); ++iter;
    EXPECT_EQ(p5, *iter); ++iter;
    EXPECT_EQ(p6, *iter); ++iter;
    EXPECT_EQ(p7, *iter); ++iter;
    EXPECT_EQ(p6, *(ch.from));
    EXPECT_EQ(p7, *(ch.to));

    // add p8
    ch.add_cw(chain, p8);
    EXPECT_EQ(4, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p2, *iter); ++iter;
    EXPECT_EQ(p5, *iter); ++iter;
    EXPECT_EQ(p8, *iter); ++iter;
    EXPECT_EQ(p5, *(ch.from));
    EXPECT_EQ(p8, *(ch.to));

}

TEST(ConvexHullGrahamYao, addCCWLeftToRight) {
    Point_2::Std_point p0(1,4,0); // elt of lower and upper hull
    Point_2::Std_point p1(3,2,1); // elt of lower hull
    Point_2::Std_point p2(4,6,2); // elt of upper hull
    Point_2::Std_point p3(5,4,3); //
    Point_2::Std_point p4(5,6,4); //
    Point_2::Std_point p5(6,7,5); // elt of upper hull
    Point_2::Std_point p6(7,5,6); //
    Point_2::Std_point p7(8,1,7); // elt of lower hull
    Point_2::Std_point p8(10,6,8); // elt of lower and upper hull

    // test computing upper hull left to right
    Convex_hull::Graham_yao ch;
    Poly_chain_2 chain;
    ch.init(chain, p0, p1);
    EXPECT_EQ(p0, *(ch.from));
    EXPECT_EQ(p1, *(ch.to));

    // add p2
    ch.add_ccw(chain, p2);
    EXPECT_EQ(3, chain.num_vertices());
    Poly_chain_2::Vertex_list::const_iterator iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p1, *iter); ++iter;
    EXPECT_EQ(p2, *iter); ++iter;
    EXPECT_EQ(p1, *(ch.from));
    EXPECT_EQ(p2, *(ch.to));

    // add p3
    ch.add_ccw(chain, p3);
    EXPECT_EQ(3, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p1, *iter); ++iter;
    EXPECT_EQ(p3, *iter); ++iter;
    EXPECT_EQ(p1, *(ch.from));
    EXPECT_EQ(p3, *(ch.to));

    // add p4
    ch.add_ccw(chain, p4);
    EXPECT_EQ(4, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p1, *iter); ++iter;
    EXPECT_EQ(p3, *iter); ++iter;
    EXPECT_EQ(p4, *iter); ++iter;
    EXPECT_EQ(p3, *(ch.from));
    EXPECT_EQ(p4, *(ch.to));

    // add p5
    ch.add_ccw(chain, p5);
    EXPECT_EQ(4, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p1, *iter); ++iter;
    EXPECT_EQ(p3, *iter); ++iter;
    EXPECT_EQ(p5, *iter); ++iter;
    EXPECT_EQ(p3, *(ch.from));
    EXPECT_EQ(p5, *(ch.to));

    // add p6
    ch.add_ccw(chain, p6);
    EXPECT_EQ(3, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p1, *iter); ++iter;
    EXPECT_EQ(p6, *iter); ++iter;
    EXPECT_EQ(p1, *(ch.from));
    EXPECT_EQ(p6, *(ch.to));

    // add p7
    ch.add_ccw(chain, p7);
    EXPECT_EQ(3, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p1, *iter); ++iter;
    EXPECT_EQ(p7, *iter); ++iter;
    EXPECT_EQ(p1, *(ch.from));
    EXPECT_EQ(p7, *(ch.to));

    // add p8
    ch.add_ccw(chain, p8);
    EXPECT_EQ(4, chain.num_vertices());
    iter = chain.vertices_begin();
    EXPECT_EQ(p0, *iter); ++iter;
    EXPECT_EQ(p1, *iter); ++iter;
    EXPECT_EQ(p7, *iter); ++iter;
    EXPECT_EQ(p8, *iter); ++iter;
    EXPECT_EQ(p7, *(ch.from));
    EXPECT_EQ(p8, *(ch.to));
}


//TEST(ConvexHullGrahamYao, UpperEnvelope) {
//    Point_2::Std_point p0(1,4,0); // elt of lower and upper hull
//    Point_2::Std_point p1(3,2,1); // elt of lower hull
//    Point_2::Std_point p2(4,6,2); // elt of upper hull
//    Point_2::Std_point p3(5,4,3); //
//    Point_2::Std_point p4(5,6,4); //
//    Point_2::Std_point p5(6,7,5); // elt of upper hull
//    Point_2::Std_point p6(7,5,6); //
//    Point_2::Std_point p7(8,1,7); // elt of lower hull
//    Point_2::Std_point p8(10,6,8); // elt of lower and upper hull
//    std::vector<Point_2::Std_point> P;
//    P.push_back(p0);
//    P.push_back(p1);
//    P.push_back(p2);
//    P.push_back(p3);
//    P.push_back(p4);
//    P.push_back(p5);
//    P.push_back(p6);
//    P.push_back(p7);
//    P.push_back(p8);
//
//    // test computing upper hull
//    Convex_hull::Graham_yao ch;
//    ch.build_upper_envelope(P);
//    EXPECT_EQ(4, chain.size());
//    Convex_hull::Graham_yao::Poly_chain::iterator iter = chain.begin();
//    EXPECT_EQ(p0, *iter); ++iter;
//    EXPECT_EQ(p1, *iter); ++iter;
//    EXPECT_EQ(p7, *iter); ++iter;
//    EXPECT_EQ(p8, *iter); ++iter;
//    EXPECT_EQ(p7, *(ch.from));
//    EXPECT_EQ(p8, *(ch.to));
//
//
//}

