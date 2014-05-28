/**
 * Filename: test_Voronoi_poly_set.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 14 Aug 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"

#include "../algos/Voronoi_poly_set.h"

#include <gtest/gtest.h>

TEST(VoronoiPolySet, aTest) {

    typedef Point_2::Std_point Point;
    Point p1(2,2, 1);
    Point p2(4,4, 2);
    Point p3(2,5, 3);

    std::vector< Point > S;
    S.push_back(p1);
    S.push_back(p2);
    S.push_back(p3);

//    const int N = 5;
    Voronoi_poly_set vps;
    std::cout << vps;

//    nn_brute.build_brute_force(S, N);
//
//    EXPECT_EQ(1, nn_brute.post_office_query(1,1));
//    EXPECT_EQ(1, nn_brute.post_office_query(2,1));
//    EXPECT_EQ(1, nn_brute.post_office_query(3,1));
//    EXPECT_EQ(1, nn_brute.post_office_query(4,1));
//    EXPECT_EQ(1, nn_brute.post_office_query(5,1));
//
//    EXPECT_EQ(1, nn_brute.post_office_query(1,2));
//    EXPECT_EQ(1, nn_brute.post_office_query(2,2));
//    EXPECT_EQ(1, nn_brute.post_office_query(3,2));
//    EXPECT_EQ(1, nn_brute.post_office_query(4,2));
//    EXPECT_EQ(2, nn_brute.post_office_query(5,2));
//
//    EXPECT_EQ(1, nn_brute.post_office_query(1,3));
//    EXPECT_EQ(1, nn_brute.post_office_query(2,3));
//    EXPECT_EQ(1, nn_brute.post_office_query(3,3));
//    EXPECT_EQ(2, nn_brute.post_office_query(4,3));
//    EXPECT_EQ(2, nn_brute.post_office_query(5,3));
//
//    EXPECT_EQ(3, nn_brute.post_office_query(1,4));
//    EXPECT_EQ(3, nn_brute.post_office_query(2,4));
//    EXPECT_EQ(2, nn_brute.post_office_query(3,4));
//    EXPECT_EQ(2, nn_brute.post_office_query(4,4));
//    EXPECT_EQ(2, nn_brute.post_office_query(5,4));
//
//    EXPECT_EQ(3, nn_brute.post_office_query(1,5));
//    EXPECT_EQ(3, nn_brute.post_office_query(2,5));
//    EXPECT_EQ(3, nn_brute.post_office_query(3,5));
//    EXPECT_EQ(2, nn_brute.post_office_query(4,5));
//    EXPECT_EQ(2, nn_brute.post_office_query(5,5));
//
//    EXPECT_TRUE(vps.verify(S, N));
}


