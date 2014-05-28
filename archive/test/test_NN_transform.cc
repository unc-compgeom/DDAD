/**
 * Filename: test_NN_transform.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 07 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */


#include "../base/DDAD_base.h"
#include "../base/Point_2.h"

#include "../algos/NN_transform.h"

#include <gtest/gtest.h>


TEST(NNTransform, buildBrute) {

    typedef Point_2::Std_point Point;
    Point p1(2,2, 1);
    Point p2(4,4, 2);
    Point p3(2,5, 3);

    std::vector< Point > S;
    S.push_back(p1);
    S.push_back(p2);
    S.push_back(p3);

    const int N = 5;
    NN_transform nn_brute;
    nn_brute.build_brute_force(S, N);

    EXPECT_EQ(1, nn_brute.post_office_query(1,1));
    EXPECT_EQ(1, nn_brute.post_office_query(2,1));
    EXPECT_EQ(1, nn_brute.post_office_query(3,1));
    EXPECT_EQ(1, nn_brute.post_office_query(4,1));
    EXPECT_EQ(1, nn_brute.post_office_query(5,1));

    EXPECT_EQ(1, nn_brute.post_office_query(1,2));
    EXPECT_EQ(1, nn_brute.post_office_query(2,2));
    EXPECT_EQ(1, nn_brute.post_office_query(3,2));
    EXPECT_EQ(1, nn_brute.post_office_query(4,2));
    EXPECT_EQ(2, nn_brute.post_office_query(5,2));

    EXPECT_EQ(1, nn_brute.post_office_query(1,3));
    EXPECT_EQ(1, nn_brute.post_office_query(2,3));
    EXPECT_EQ(1, nn_brute.post_office_query(3,3));
    EXPECT_EQ(2, nn_brute.post_office_query(4,3));
    EXPECT_EQ(2, nn_brute.post_office_query(5,3));

    EXPECT_EQ(3, nn_brute.post_office_query(1,4));
    EXPECT_EQ(3, nn_brute.post_office_query(2,4));
    EXPECT_EQ(2, nn_brute.post_office_query(3,4));
    EXPECT_EQ(2, nn_brute.post_office_query(4,4));
    EXPECT_EQ(2, nn_brute.post_office_query(5,4));

    EXPECT_EQ(3, nn_brute.post_office_query(1,5));
    EXPECT_EQ(3, nn_brute.post_office_query(2,5));
    EXPECT_EQ(3, nn_brute.post_office_query(3,5));
    EXPECT_EQ(2, nn_brute.post_office_query(4,5));
    EXPECT_EQ(2, nn_brute.post_office_query(5,5));

    EXPECT_TRUE(nn_brute.verify(S, N));
}


TEST(NNTransform, buildD2USq01) {

    typedef Point_2::Std_point Point;
    std::vector< Point > S;
    S.push_back(Point(2,2, 1));
    S.push_back(Point(4,4, 2));
    S.push_back(Point(2,5, 3));

    const int N = 5;
    NN_transform nn_d2_usq;
    nn_d2_usq.build_deg_2_usq(S, N);

    EXPECT_EQ(1, nn_d2_usq.post_office_query(1,1));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(2,1));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(3,1));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(4,1));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(5,1));

    EXPECT_EQ(1, nn_d2_usq.post_office_query(1,2));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(2,2));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(3,2));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(4,2));
    EXPECT_EQ(2, nn_d2_usq.post_office_query(5,2));

    EXPECT_EQ(1, nn_d2_usq.post_office_query(1,3));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(2,3));
    EXPECT_EQ(1, nn_d2_usq.post_office_query(3,3));
    EXPECT_EQ(2, nn_d2_usq.post_office_query(4,3));
    EXPECT_EQ(2, nn_d2_usq.post_office_query(5,3));

    EXPECT_EQ(3, nn_d2_usq.post_office_query(1,4));
    EXPECT_EQ(3, nn_d2_usq.post_office_query(2,4));
    EXPECT_EQ(2, nn_d2_usq.post_office_query(3,4));
    EXPECT_EQ(2, nn_d2_usq.post_office_query(4,4));
    EXPECT_EQ(2, nn_d2_usq.post_office_query(5,4));

    EXPECT_EQ(3, nn_d2_usq.post_office_query(1,5));
    EXPECT_EQ(3, nn_d2_usq.post_office_query(2,5));
    EXPECT_EQ(3, nn_d2_usq.post_office_query(3,5));
    EXPECT_EQ(2, nn_d2_usq.post_office_query(4,5));
    EXPECT_EQ(2, nn_d2_usq.post_office_query(5,5));

    EXPECT_TRUE(nn_d2_usq.verify(S, N));
}


TEST(NNTransform, buildD2USq02) {

    typedef Point_2::Std_point Point;
    std::vector< Point > S;
    S.push_back(Point(4,4, 5));
    S.push_back(Point(2,5, 2));
    S.push_back(Point(4,2, 3));
    S.push_back(Point(2,2, 1));
    S.push_back(Point(5,3, 6));
    S.push_back(Point(4,3, 4));
    S.push_back(Point(5,5, 7));

    const int N = 5;
    NN_transform nn_d2_usq;
    nn_d2_usq.build_deg_2_usq(S, N);
    EXPECT_TRUE(nn_d2_usq.verify(S,N));
}


TEST(NNTransform, causedErrors01) {
    DDAD_goodies::seed_rand(1331607797);
    typedef Point_2::Std_point Point;
    std::vector< Point > S;
    S.push_back(Point(8, 8,  6));
    S.push_back(Point(6, 6,  7));
    S.push_back(Point(5, 2,  8));
    S.push_back(Point(2, 1,  9));

    NN_transform nn_d2_usq;
    nn_d2_usq.build_deg_2_usq(S, 8);
    EXPECT_TRUE(nn_d2_usq.verify(S,8));
}


TEST(NNTransform, causedErrors02) {
    DDAD_goodies::seed_rand(1331608452);
    typedef Point_2::Std_point Point;
    std::vector< Point > S;

    S.push_back(Point(6, 6,  7));
    S.push_back(Point(2, 1,  9));
    S.push_back(Point(4, 2, 11));
    S.push_back(Point(1, 2, 12));
    S.push_back(Point(7, 5, 13));
    S.push_back(Point(3, 5, 14));
    S.push_back(Point(5, 6, 15));
    S.push_back(Point(8, 4, 16));

    NN_transform nn_d2_usq;
    nn_d2_usq.build_deg_2_usq(S, 8);
    EXPECT_TRUE(nn_d2_usq.verify(S,8));
}
