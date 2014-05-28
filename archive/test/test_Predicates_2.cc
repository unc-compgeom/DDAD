/**
 * Filename: test_Predicates_2.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 08 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "../base/Predicates_2.h"

#include <gtest/gtest.h>


TEST(Predicates2, inClosedInterval) {
    EXPECT_TRUE(Predicates_2::in_closed_interval(0,3,0));
    EXPECT_TRUE(Predicates_2::in_closed_interval(0,3,3));
    EXPECT_TRUE(Predicates_2::in_closed_interval(0,3,2));
    EXPECT_FALSE(Predicates_2::in_closed_interval(0,3,4));
    EXPECT_FALSE(Predicates_2::in_closed_interval(0,3,-1));
}



TEST(Predicates2, Above) {
    Point_2::Std_point p(0, 0);
    Point_2::Std_point q(1, 1);

    Point_2::Std_point ra(2, 3);
    Point_2::Std_point rb(2, 1);

    Point_2::Std_point pz(0, 0);
    Point_2::Std_point qz(0, 2);
    Point_2::Std_point rz(2, 2);

    EXPECT_TRUE(Predicates_2::above(Segment_2::Std_segment(p,q),ra));
    EXPECT_TRUE(Predicates_2::above(Segment_2::Std_segment(q,p),ra));
    EXPECT_FALSE(Predicates_2::above(Segment_2::Std_segment(p,q),rb));
    EXPECT_FALSE(Predicates_2::above(Segment_2::Std_segment(q,p),rb));
    EXPECT_FALSE(Predicates_2::above(Segment_2::Std_segment(p,q),rz));

    // below should thow an assertion
    //EXPECT_FALSE(Predicates_2::above(Segment_2::Std_segment(pz,qz),rz));
}


TEST(Predicates2, Below) {
    Point_2::Std_point p(0, 0);
    Point_2::Std_point q(1, 1);

    Point_2::Std_point ra(2, 3);
    Point_2::Std_point rb(2, 1);

    Point_2::Std_point pz(0, 0);
    Point_2::Std_point qz(0, 2);
    Point_2::Std_point rz(2, 2);

    EXPECT_FALSE(Predicates_2::below(Segment_2::Std_segment(p,q),ra));
    EXPECT_FALSE(Predicates_2::below(Segment_2::Std_segment(q,p),ra));
    EXPECT_TRUE( Predicates_2::below(Segment_2::Std_segment(p,q),rb));
    EXPECT_TRUE( Predicates_2::below(Segment_2::Std_segment(q,p),rb));
    EXPECT_FALSE(Predicates_2::above(Segment_2::Std_segment(p,q),rz));

    // below should thow an assertion
    //EXPECT_FALSE(Predicates_2::above(Segment_2::Std_segment(pz,qz),rz));
}


TEST(Predicates2, OnOpenSegment) {

    Segment_2::Std_segment s(Point_2::Std_point(0, 0), Point_2::Std_point(2, 2));

    Point_2::Std_point t(1, 1);
    Point_2::Std_point f0(3, 2);
    Point_2::Std_point f1(2, 2);
    Point_2::Std_point f2(0, 0);
    Point_2::Std_point f3(3, 3);

    EXPECT_TRUE(Predicates_2::on_open_segment(s ,t));
    EXPECT_FALSE(Predicates_2::on_open_segment(s ,f0));
    EXPECT_FALSE(Predicates_2::on_open_segment(s ,f1));
    EXPECT_FALSE(Predicates_2::on_open_segment(s ,f2));
    EXPECT_FALSE(Predicates_2::on_open_segment(s ,f3));
}



TEST(Predicates2, OnClosedSegment) {

    Segment_2::Std_segment s(Point_2::Std_point(0, 0), Point_2::Std_point(2, 2));

    Point_2::Std_point t(1, 1);
    Point_2::Std_point f0(3, 2);
    Point_2::Std_point f1(2, 2);
    Point_2::Std_point f2(0, 0);
    Point_2::Std_point f3(3, 3);

    EXPECT_TRUE( Predicates_2::on_closed_segment(s ,t));
    EXPECT_FALSE(Predicates_2::on_closed_segment(s ,f0));
    EXPECT_TRUE( Predicates_2::on_closed_segment(s ,f1));
    EXPECT_TRUE( Predicates_2::on_closed_segment(s ,f2));
    EXPECT_FALSE(Predicates_2::on_closed_segment(s ,f3));
}



TEST(Predicates2, IntersectInterior) {
    typedef Segment_2::Std_segment Seg2;
    typedef Seg2::Std_segment Sg;
    typedef Point_2::Std_point Pt;
    typedef Predicates_2 Pred;
    Sg s1(Pt(4, 4), Pt(0,0));

    EXPECT_EQ(Pred::SEG_INTER_POINT,
            Pred::intersect_interior(s1, Sg(Pt(0,2), Pt(2,0))));
    EXPECT_EQ(Pred::SEG_INTER_EMPTY,
            Pred::intersect_interior(s1, Sg(Pt(0,2), Pt(1,2))));
    EXPECT_EQ(Pred::SEG_INTER_EMPTY,
            Pred::intersect_interior(s1, Sg(Pt(0,0), Pt(0,2))));
    EXPECT_EQ(Pred::SEG_INTER_EMPTY,
            Pred::intersect_interior(s1, Sg(Pt(2,2), Pt(2,1))));
    EXPECT_EQ(Pred::SEG_INTER_EMPTY,
            Pred::intersect_interior(s1, Sg(Pt(4,4), Pt(4,5))));

    EXPECT_EQ(Pred::SEG_INTER_OPEN_SEG,
            Pred::intersect_interior(s1, Sg(Pt(1,1), Pt(3,3))));
    EXPECT_EQ(Pred::SEG_INTER_OPEN_SEG,
            Pred::intersect_interior(s1, Sg(Pt(4,4), Pt(3,3))));
    EXPECT_EQ(Pred::SEG_INTER_OPEN_SEG,
            Pred::intersect_interior(s1, Sg(Pt(5,5), Pt(3,3))));
    EXPECT_EQ(Pred::SEG_INTER_EMPTY,
            Pred::intersect_interior(s1, Sg(Pt(4,4), Pt(5,5))));
    EXPECT_EQ(Pred::SEG_INTER_EMPTY,
            Pred::intersect_interior(s1, Sg(Pt(6,6), Pt(5,5))));
}







