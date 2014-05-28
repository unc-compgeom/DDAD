/**
 * Filename: test_Constructions_2.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 18 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "../base/Constructions_2.h"

#include <gtest/gtest.h>

TEST(Constructions2, CoordsForStdPoint ) {
    Point_2::Std_point p(0, 1);
    Point_2::Std_point q(2, 3);
    Point_2::Std_point r(-1, -2);

    float x,y;
    Constructions_2::coords(x,y,p);
    EXPECT_EQ(0,x);
    EXPECT_EQ(1,y);

    Constructions_2::coords(x,y,q);
    EXPECT_EQ(2,x);
    EXPECT_EQ(3,y);

    Constructions_2::coords(x,y,r);
    EXPECT_EQ(-1,x);
    EXPECT_EQ(-2,y);

    EXPECT_EQ(0,Constructions_2::coord_x(p));
    EXPECT_EQ(1,Constructions_2::coord_y(p));
    EXPECT_EQ(2, Constructions_2::coord_y_mid_point(p,q));
}


