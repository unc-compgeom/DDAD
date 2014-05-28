/**
 * Filename: test_Line_2.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 27 Jun 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "../base/Line_2.h"

#include <gtest/gtest.h>


TEST(Line2, MandB_XIntesectionOrder) {

    Line_2::Slope_and_y_intercept l0(0, 4);
    Line_2::Slope_and_y_intercept l1(2, 8);
    Line_2::Slope_and_y_intercept l2(4, 1);
    Line_2::Slope_and_y_intercept l3(1, 6);
    Line_2::Slope_and_y_intercept l4(3, 10);

    // 3 pts
    EXPECT_EQ(Line_2::X_ORDER_LEFT,
            Line_2::x_intersection_order(l0,l1,l2));

    EXPECT_EQ(Line_2::X_ORDER_RIGHT,
            Line_2::x_intersection_order(l0,l2,l1));

    EXPECT_EQ(Line_2::X_ORDER_LEFT,
            Line_2::x_intersection_order(l0,l3,l2));

    EXPECT_EQ(Line_2::X_ORDER_RIGHT,
            Line_2::x_intersection_order(l0,l2,l3));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l0,l1,l3));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l0,l3,l3));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l0,l1,l1));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l0,l2,l2));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l0,l3,l3));



    // 4 pts
    EXPECT_EQ(Line_2::X_ORDER_LEFT,
            Line_2::x_intersection_order(l1,l3,l0,l2));

    EXPECT_EQ(Line_2::X_ORDER_LEFT,
            Line_2::x_intersection_order(l3,l1,l0,l2));

    EXPECT_EQ(Line_2::X_ORDER_LEFT,
            Line_2::x_intersection_order(l3,l1,l0,l2));

    EXPECT_EQ(Line_2::X_ORDER_LEFT,
            Line_2::x_intersection_order(l3,l1,l2,l0));


    EXPECT_EQ(Line_2::X_ORDER_RIGHT,
            Line_2::x_intersection_order(l0,l2,l1,l3));

    EXPECT_EQ(Line_2::X_ORDER_RIGHT,
            Line_2::x_intersection_order(l2,l0,l3,l1));

    EXPECT_EQ(Line_2::X_ORDER_RIGHT,
            Line_2::x_intersection_order(l0,l2,l3,l1));

    EXPECT_EQ(Line_2::X_ORDER_RIGHT,
            Line_2::x_intersection_order(l2,l0,l1,l3));


    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l0,l1,l3,l4));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l1,l0,l3,l4));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l0,l1,l4,l3));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l1,l0,l4,l3));

    EXPECT_EQ(Line_2::X_ORDER_COINCIDENT,
            Line_2::x_intersection_order(l0,l3,l4,l1));
}


TEST(Line2, MandB_YOnLeftBoundary) {

    Line_2::Slope_and_y_intercept l0(5, 0);
    Line_2::Slope_and_y_intercept l1(2, 0);
    Line_2::Slope_and_y_intercept l2(2, 1);
    Line_2::Slope_and_y_intercept l3(1, 4);

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_order_on_left_boundary(l0,l1));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_order_on_left_boundary(l0,l2));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_order_on_left_boundary(l0,l3));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_order_on_left_boundary(l1,l2));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_order_on_left_boundary(l1,l3));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_order_on_left_boundary(l2,l3));


    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_order_on_left_boundary(l3,l0));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_order_on_left_boundary(l3,l1));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_order_on_left_boundary(l3,l2));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_order_on_left_boundary(l2,l1));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_order_on_left_boundary(l2,l0));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_order_on_left_boundary(l1,l0));
}


TEST(Line2, MandB_Yorder) {

    Line_2::Slope_and_y_intercept l0(2, 1);
    Line_2::Slope_and_y_intercept l1(3, 2);

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_intersection_order(l0,l1,-2));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_intersection_order(l1,l0,-2));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_intersection_order(l1,l0,0));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_intersection_order(l0,l1,0));

    EXPECT_EQ(Line_2::Y_ORDER_COINCIDENT,
            Line_2::y_intersection_order(l1,l0,-1));

    EXPECT_EQ(Line_2::Y_ORDER_COINCIDENT,
            Line_2::y_intersection_order(l0,l1,-1));
}



TEST(Line2, MandB_verticalGridLineLeftOrThourghIntersection) {

    Line_2::Slope_and_y_intercept l0(2, 1);
    Line_2::Slope_and_y_intercept l1(3, 2);
    Line_2::Slope_and_y_intercept l2(4, 0);
    Line_2::Slope_and_y_intercept l3(4, 4);

    int val;
    EXPECT_EQ(Line_2::IS_NOT_PARALLEL,
            Line_2::vertical_grid_line_left_or_thourgh_intersection(val,l0,l1));
    EXPECT_EQ(-1, val);

    EXPECT_EQ(Line_2::IS_NOT_PARALLEL,
            Line_2::vertical_grid_line_left_or_thourgh_intersection(val,l1,l0));
    EXPECT_EQ(-1, val);

    EXPECT_EQ(Line_2::IS_NOT_PARALLEL,
            Line_2::vertical_grid_line_left_or_thourgh_intersection(val,l2,l0));
    EXPECT_EQ(0,val);
    EXPECT_EQ(Line_2::IS_NOT_PARALLEL,
            Line_2::vertical_grid_line_left_or_thourgh_intersection(val,l0,l2));
    EXPECT_EQ(0,val);

    val = 100;
    EXPECT_EQ(Line_2::IS_PARALLEL,
            Line_2::vertical_grid_line_left_or_thourgh_intersection(val,l2,l3));
    EXPECT_EQ(100,val);
    EXPECT_EQ(Line_2::IS_PARALLEL,
            Line_2::vertical_grid_line_left_or_thourgh_intersection(val,l3,l2));
    EXPECT_EQ(100,val);


}


TEST(Line2, MandB_slopeOrder) {

    Line_2::Slope_and_y_intercept l0(2, 1);
    Line_2::Slope_and_y_intercept l1(2, 2);
    Line_2::Slope_and_y_intercept l2(4, 0);

    EXPECT_EQ(Line_2::SLOPE_ORDER_LESS, Line_2::slope_order(l0,l2));
    EXPECT_EQ(Line_2::SLOPE_ORDER_GREATER, Line_2::slope_order(l2,l0));
    EXPECT_EQ(Line_2::SLOPE_ORDER_SAME, Line_2::slope_order(l0,l1));
    EXPECT_EQ(Line_2::SLOPE_ORDER_SAME, Line_2::slope_order(l1,l0));
}



TEST(Line2, MandB_isParalle) {

    Line_2::Slope_and_y_intercept l0(5, 0);
    Line_2::Slope_and_y_intercept l1(5, 1);
    Line_2::Slope_and_y_intercept l2(2, 1);

    EXPECT_EQ(Line_2::IS_PARALLEL,
            Line_2::is_parallel(l0,l1));

    EXPECT_EQ(Line_2::IS_NOT_PARALLEL,
            Line_2::is_parallel(l0,l2));
}


TEST(Line2, MandB_YIntesectionOrder) {

    Line_2::Slope_and_y_intercept l0(0, 4);
    Line_2::Slope_and_y_intercept l1(2, 8);
    Line_2::Slope_and_y_intercept l2(4, 1);
    Line_2::Slope_and_y_intercept l3(1, 6);
    Line_2::Slope_and_y_intercept l4(3, 10);

    // 4 pts
    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_intersection_order(l2,l3,l0,l1));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_intersection_order(l3,l2,l0,l1));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_intersection_order(l2,l3,l1,l0));

    EXPECT_EQ(Line_2::Y_ORDER_ABOVE,
            Line_2::y_intersection_order(l3,l2,l1,l0));


    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_intersection_order(l0,l1,l2,l3));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_intersection_order(l1,l0,l2,l3));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_intersection_order(l0,l1,l3,l2));

    EXPECT_EQ(Line_2::Y_ORDER_BELOW,
            Line_2::y_intersection_order(l1,l0,l3,l2));


    EXPECT_EQ(Line_2::Y_ORDER_COINCIDENT,
            Line_2::y_intersection_order(l0,l1,l3,l4));

    EXPECT_EQ(Line_2::Y_ORDER_COINCIDENT,
            Line_2::y_intersection_order(l1,l0,l3,l4));

    EXPECT_EQ(Line_2::Y_ORDER_COINCIDENT,
            Line_2::y_intersection_order(l0,l1,l4,l3));

    EXPECT_EQ(Line_2::Y_ORDER_COINCIDENT,
            Line_2::y_intersection_order(l1,l0,l4,l3));

    EXPECT_EQ(Line_2::Y_ORDER_COINCIDENT,
            Line_2::y_intersection_order(l0,l3,l4,l1));
}




TEST(Line2, MandB_simpleConstructors) {

    Line_2::Slope_and_y_intercept l0(2, 1);
    Line_2::Slope_and_y_intercept l1(2, 2);
    Line_2::Slope_and_y_intercept l2(4, 0);

    EXPECT_EQ(2, Line_2::slope(l0));
    EXPECT_EQ(2, Line_2::slope(l1));
    EXPECT_EQ(4, Line_2::slope(l2));

    EXPECT_EQ(1, Line_2::y_intercept(l0));
    EXPECT_EQ(2, Line_2::y_intercept(l1));
    EXPECT_EQ(0, Line_2::y_intercept(l2));

    l0.reset(3,4, 8);
    EXPECT_EQ(3, Line_2::slope(l0));
    EXPECT_EQ(4, Line_2::y_intercept(l0));
    EXPECT_EQ(8, l0.id());

}





