/**
 * Filename: test_Point_2.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 08 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "../base/Point_2.h"

#include <gtest/gtest.h>

TEST(Predicates2, Orientation) {
    Point_2::Std_point p(0, 0);
    Point_2::Std_point q(1, 0);
    Point_2::Std_point r(1, 1);
    Point_2::Std_point rr(1, -1);
    Point_2::Std_point rz(2, 0);
    EXPECT_EQ(Point_2::ORIENT_LEFT,   Point_2::orientation(p,q,r));
    EXPECT_EQ(Point_2::ORIENT_RIGHT,  Point_2::orientation(p,q,rr));
    EXPECT_EQ(Point_2::ORIENT_COLINE, Point_2::orientation(p,q,rz));
}


TEST(Predicates2, Closer) {
    Point_2::Std_point a(0, 0);
    Point_2::Std_point b(1, -1);
    Point_2::Std_point qa(0, 1);
    Point_2::Std_point qb(2, 0);
    Point_2::Std_point qz(1, 0);
    EXPECT_EQ(Point_2::CLOSER_TO_FIRST,    Point_2::closer(a,b,qa));
    EXPECT_EQ(Point_2::CLOSER_TO_SECOND,   Point_2::closer(a,b,qb));
    EXPECT_EQ(Point_2::CLOSER_EQUIDISTANT, Point_2::closer(a,b,qz));
}

TEST(Predicates2, OnVerticalLine) {
    Point_2::Std_point p(0, 0);
    Point_2::Std_point q(1, 0);
    Point_2::Std_point r(1, 1);
    EXPECT_FALSE(Point_2::on_vertical_line(p,q));
    EXPECT_FALSE(Point_2::on_vertical_line(p,r));
    EXPECT_TRUE( Point_2::on_vertical_line(q,r));
}


TEST(Predicates2, Precedes) {
    Point_2::Std_point p(0, 0);
    Point_2::Std_point q(1, 0);
    Point_2::Std_point r(1, 1);
    EXPECT_TRUE(Point_2::precedes(p,q));
    EXPECT_TRUE(Point_2::precedes(p,r));
    EXPECT_TRUE(Point_2::precedes(q,r));

    EXPECT_FALSE(Point_2::precedes(q,p));
    EXPECT_FALSE(Point_2::precedes(r,p));
    EXPECT_FALSE(Point_2::precedes(r,q));

}


