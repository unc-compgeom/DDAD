/**
 * Filename: Line_2.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 27 Jun 2011
 * @author: David L. Millman <dave@cs.unc.edu>
 */

#include "Line_2.h"

Line_2::X_order Line_2::x_intersection_order(
        const Slope_and_y_intercept& l0,
        const Slope_and_y_intercept& l1,
        const Slope_and_y_intercept& l2) {

    // verify that the lines are distinct
    rAssert(l0 != l1 && l0 != l2);
    rAssert(IS_NOT_PARALLEL == is_parallel(l0,l1));
    rAssert(IS_NOT_PARALLEL == is_parallel(l0,l2));

    return x_intersection_order(l1, l0, l2, l0);
}


Line_2::X_order Line_2::x_intersection_order(
        const Slope_and_y_intercept& l0,
        const Slope_and_y_intercept& l1,
        const Slope_and_y_intercept& l2,
        const Slope_and_y_intercept& l3) {

    rAssert(l0 != l1 && l2 != l3
            && IS_NOT_PARALLEL == is_parallel(l0,l1)
            && IS_NOT_PARALLEL == is_parallel(l2,l3));

    int dm01 = l0.m() - l1.m();
    int db10 = l1.b() - l0.b();

    int dm23 = l2.m() - l3.m();
    int db32 = l3.b() - l2.b();

    int num = db10*dm23 - dm01*db32;
    num *= (dm01 < 0)?-1:1;
    num *= (dm23 < 0)?-1:1;

    if (num == 0) { return X_ORDER_COINCIDENT; }
    else if (num < 0) { return X_ORDER_LEFT; }
    else { rAssert(num>0); return X_ORDER_RIGHT; }
}



Line_2::Parallel Line_2::is_parallel(
        const Slope_and_y_intercept& l0,
        const Slope_and_y_intercept& l1) {
    return (l0.m() == l1.m()) ? IS_PARALLEL : IS_NOT_PARALLEL;
}


Line_2::Y_order Line_2::y_intersection_order(
        const Slope_and_y_intercept& l0,
        const Slope_and_y_intercept& l1,
        int x0) {

    int e0 = l0.eval(x0);
    int e1 = l1.eval(x0);

    if (e0 > e1) { return Y_ORDER_ABOVE; }
    else if (e0 < e1) { return Y_ORDER_BELOW; }
    else { rAssert(e0 == e1); return Y_ORDER_COINCIDENT; }
}



Line_2::Cell_inter_type Line_2::classify_intersection(
        const Slope_and_y_intercept& l,
        const Slope_and_y_intercept& m, int left, int right) {


    int ll = l.eval(left);
    int lr = l.eval(right);
    int ml = m.eval(left);
    int mr = m.eval(right);

    // check missed intersection
    // and for 1 point wide cell
    if (ll < ml && lr < mr)  { return CELL_INTER_EMPTY; }
    else if (left == right && ll == ml) { return CELL_INTER_RIGHT_AND_LEFT_BDD; }

    // if we got here we know the line intersects the cell.
    // Now we need to determine if it is in the interior
    // or the left, right or lower boundary
    if (ll == ml) { return CELL_INTER_LEFT_BDD; }
    else if (lr == mr) { return CELL_INTER_RIGHT_BDD; }
    else if (ll < ml || lr < mr) { return CELL_INTER_LOWER_BDD; }
    else { return CELL_INTER_INTERIOR; }
}

bool Line_2::interior_intersection(const Slope_and_y_intercept& l,
        const Slope_and_y_intercept& m, int left, int right) {
    return (l.eval(left) > m.eval(left) && l.eval(right) > m.eval(right));
}

Line_2::Y_order Line_2::y_intersection_order(
        const Slope_and_y_intercept& l0,
        const Slope_and_y_intercept& l1,
        const Slope_and_y_intercept& l2,
        const Slope_and_y_intercept& l3) {

    rAssert(l0 != l1 && l2 != l3
            && IS_NOT_PARALLEL == is_parallel(l0,l1)
            && IS_NOT_PARALLEL == is_parallel(l2,l3));

    int dm01 = l0.m() - l1.m();
    int db10 = l1.b() - l0.b();

    int dm23 = l2.m() - l3.m();
    int db32 = l3.b() - l2.b();

    int num = dm23*(db10*l0.m() + l0.b()*dm01)
        - dm01*(db32*l2.m() + dm23*l2.b());
    num *= (dm01 < 0)?-1:1;
    num *= (dm23 < 0)?-1:1;

    if (num == 0) { return Y_ORDER_COINCIDENT; }
    else if (num > 0) { return Y_ORDER_ABOVE; }
    else { rAssert(num<0); return Y_ORDER_BELOW; }
}


Line_2::Y_order Line_2::y_order_on_left_boundary(
        const Slope_and_y_intercept& l0,
        const Slope_and_y_intercept& l1) {

    rAssert(l0 != l1);

    if (l0.m() > l1.m()
           || (l0.m() == l1.m() && l0.b() < l1.b()) ) {
        return Y_ORDER_BELOW;
    } else {
        rAssert(l0.m() < l1.m()
                || (l0.m() == l1.m() && l0.b() > l1.b()));
        return Y_ORDER_ABOVE;
    }
}

Line_2::Y_order Line_2::y_order_on_right_boundary(
        const Slope_and_y_intercept& l0,
        const Slope_and_y_intercept& l1) {

    rAssert(l0 != l1);

    if (l0.m() < l1.m()
           || (l0.m() == l1.m() && l0.b() < l1.b()) ) {
        return Y_ORDER_BELOW;
    } else {
        rAssert(l0.m() > l1.m()
                || (l0.m() == l1.m() && l0.b() > l1.b()));
        return Y_ORDER_ABOVE;
    }
}


Line_2::Parallel Line_2::intersection_coords(float& x, float& y,
            const Slope_and_y_intercept& l0,
            const Slope_and_y_intercept& l1) {

    if (IS_PARALLEL == is_parallel(l0,l1)) { return IS_PARALLEL; }

    x = (l0.b() - l1.b())/((float)(l1.m()-l0.m()));
    y = l0.m()*x + l0.b();

    return IS_NOT_PARALLEL;
}


Line_2::Parallel Line_2::intersection_coords(float& x, float& y,
        const Slope_and_y_intercept& l0, const Vertical& l1) {
    x = l1.x_int();
    y = l0.m()*x + l0.b();
    return IS_NOT_PARALLEL;
}



Line_2::Parallel Line_2::vertical_grid_line_left_or_thourgh_intersection(int& x,
        const Slope_and_y_intercept& l0, const Slope_and_y_intercept& l1) {
    if (l0.m() == l1.m()) {
        return IS_PARALLEL;
    } else {
        x = (int)floor( (float)(l1.b()-l0.b()) / (l0.m()-l1.m()) );
        return IS_NOT_PARALLEL;
    }
}


Line_2::Slope_order Line_2::slope_order( const Slope_and_y_intercept& l0,
        const Slope_and_y_intercept& l1) {
    if (l0.m() < l1.m()) { return SLOPE_ORDER_LESS; }
    else if (l0.m() > l1.m()) { return SLOPE_ORDER_GREATER; }
    else { rAssert(l0.m() == l1.m()); return SLOPE_ORDER_SAME; }
}


Point_2::Std_point Line_2::dual(const Slope_and_y_intercept& l0, int id) {

    int use_id = (id==-1) ? l0.id() : id;
    return Point_2::Std_point(l0.m(), -l0.b(), use_id, (void*)& l0);
}

int Line_2::slope(const Slope_and_y_intercept& l0) { return l0.m(); }


int Line_2::y_intercept(const Slope_and_y_intercept& l0) { return l0.b(); }


