/**
 * Filename: test_Discrete_upper_envelope.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 21 Feb 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "../base/DDAD_base.h"

#include "../algos/Discrete_upper_envelope.h"

#include <gtest/gtest.h>

TEST(DiscreteUpperEnvelope, buildBruteForce) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    // create lines
    Line l0(2, 98, 0);
    Line l1(4, 95, 1);
    Line l11(4, 1, 11);
    Line l2(8, 79, 2);
    Line l22(8, 3, 22);
    Line l3(10, 59, 3);
    Line l4(12, 63, 4);
    Line l5(14, 35, 5);
    Line l6(20, 0, 6);

    // push lines on to structure
    std::vector< Line > l;
    l.push_back(l0);
    l.push_back(l1);
    l.push_back(l11);
    l.push_back(l2);
    l.push_back(l22);
    l.push_back(l3);
    l.push_back(l4);
    l.push_back(l5);
    l.push_back(l6);
    std::random_shuffle(l.begin(), l.end());

    // build the discrete upper envelope
    Discrete_upper_envelope due;
    due.build_brute_force(l, 1, 10);

    // check with "known good result"
    EXPECT_EQ(4, due.size());
    Envelope_iter i = due.begin();
    EXPECT_EQ(*i, Envelope_cell(l0, 1,1)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l1, 2,4)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l4, 5,7)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l6, 8,10)); ++i;
    EXPECT_TRUE(i == due.end());
}

TEST(DiscreteUpperEnvelope, buildOrient) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    // create lines
    Line l0(2, 98, 0);
    Line l1(4, 95, 1);
    Line l11(4, 1, 11);
    Line l2(8, 79, 2);
    Line l22(8, 3, 22);
    Line l3(10, 59, 3);
    Line l4(12, 63, 4);
    Line l5(14, 35, 5);
    Line l6(20, 0, 6);

    // push lines on to structure
    std::vector< Line > l;
    l.push_back(l0);
    l.push_back(l1);
    l.push_back(l11);
    l.push_back(l2);
    l.push_back(l22);
    l.push_back(l3);
    l.push_back(l4);
    l.push_back(l5);
    l.push_back(l6);
    std::random_shuffle(l.begin(), l.end());

    // build the discrete upper envelope with orientations
    Discrete_upper_envelope due_orient;
    due_orient.build_orient(l, 1, 10);

    // build the discrete upper envelope via brute force
    Discrete_upper_envelope due_brute;
    due_brute.build_brute_force(l, 1, 10);

    // check with "known good result"
    EXPECT_EQ(4, due_brute.size());
    Envelope_iter i = due_brute.begin();
    EXPECT_EQ(*i, Envelope_cell(l0, 1,1)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l1, 2,4)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l4, 5,7)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l6, 8,10)); ++i;
    EXPECT_TRUE(i == due_brute.end());

    // make sure the results match
    EXPECT_EQ(due_brute.size(), due_orient.size());
    Envelope_iter o = due_orient.begin();
    Envelope_iter b = due_brute.begin();
    while (o != due_orient.end()) {
        EXPECT_EQ(b->line, o->line);
        EXPECT_EQ(b->left, o->left);
        EXPECT_EQ(b->right, o->right);
        ++o;
        ++b;
    }
    EXPECT_TRUE(o == due_orient.end());
    EXPECT_TRUE(b == due_brute.end());
}


TEST(DiscreteUpperEnvelope, buildULogU) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    // create lines
    Line l0(2, 98, 0);
    Line l1(4, 95, 1);
    Line l11(4, 1, 11);
    Line l2(8, 79, 2);
    Line l22(8, 3, 22);
    Line l3(10, 59, 3);
    Line l4(12, 63, 4);
    Line l5(14, 35, 5);
    Line l6(20, 0, 6);

    // push lines on to structure
    std::vector< Line > l;
    l.push_back(l0);
    l.push_back(l1);
    l.push_back(l11);
    l.push_back(l2);
    l.push_back(l22);
    l.push_back(l3);
    l.push_back(l4);
    l.push_back(l5);
    l.push_back(l6);
    std::random_shuffle(l.begin(), l.end());

    // build the discrete upper envelope with u log u algorithm
    Discrete_upper_envelope due_ulogu;
    due_ulogu.build_ulogu(l, 1, 10);

    // build the discrete upper envelope via brute force
    Discrete_upper_envelope due_brute;
    due_brute.build_brute_force(l, 1, 10);

    // check with "known good result"
    EXPECT_EQ(4, due_brute.size());
    Envelope_iter i = due_brute.begin();
    EXPECT_EQ(*i, Envelope_cell(l0, 1,1)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l1, 2,4)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l4, 5,7)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l6, 8,10)); ++i;
    EXPECT_TRUE(i == due_brute.end());

    // make sure the results match
    EXPECT_EQ(due_brute.size(), due_ulogu.size());
    Envelope_iter o = due_ulogu.begin();
    Envelope_iter b = due_brute.begin();
    while (o != due_ulogu.end()) {
        EXPECT_EQ(b->line, o->line);
        EXPECT_EQ(b->left, o->left);
        EXPECT_EQ(b->right, o->right);
        ++o;
        ++b;
    }
    EXPECT_TRUE(o == due_ulogu.end());
    EXPECT_TRUE(b == due_brute.end());
}

TEST(DiscreteUpperEnvelope, buildRIC) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    DDAD_goodies::seed_rand(6);
    // create lines
    Line l0(2, 98, 0);
    Line l1(4, 95, 1);
    Line l11(4, 1, 11);
    Line l2(8, 79, 2);
    Line l22(8, 3, 22);
    Line l3(10, 59, 3);
    Line l4(12, 63, 4);
    Line l5(14, 35, 5);
    Line l6(20, 0, 6);

    // push lines on to structure
    std::vector< Line > l;
    l.push_back(l0);
    l.push_back(l1);
    l.push_back(l11);
    l.push_back(l2);
    l.push_back(l22);
    l.push_back(l3);
    l.push_back(l4);
    l.push_back(l5);
    l.push_back(l6);
    std::random_shuffle(l.begin(), l.end());

    // build the discrete upper envelope with RIC
    Discrete_upper_envelope due_ric;
    due_ric.build_ric(l, 1, 10);

    // build the discrete upper envelope via brute force
    Discrete_upper_envelope due_brute;
    due_brute.build_brute_force(l, 1, 10);

    // check with "known good result"
    EXPECT_EQ(4, due_brute.size());
    Envelope_iter i = due_brute.begin();
    EXPECT_EQ(*i, Envelope_cell(l0, 1,1)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l1, 2,4)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l4, 5,7)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l6, 8,10)); ++i;
    EXPECT_TRUE(i == due_brute.end());


    // make sure the results match
    EXPECT_EQ(due_brute.size(), due_ric.size());
    Envelope_iter o = due_ric.begin();
    Envelope_iter b = due_brute.begin();
    while (o != due_ric.end()) {
        EXPECT_EQ(b->line, o->line);
        EXPECT_EQ(b->left, o->left);
        EXPECT_EQ(b->right, o->right);
        ++o;
        ++b;
    }
    EXPECT_TRUE(o == due_ric.end());
    EXPECT_TRUE(b == due_brute.end());

}

TEST(DiscreteUpperEnvelope, causedErrors01) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    // create lines
    Line l2(8, 79, 2);
    Line l4(12, 63, 4);
    Line l6(20, 0, 6);

    // push lines on to structure
    std::vector< Line > l;
    l.push_back(l2);
    l.push_back(l4);
    l.push_back(l6);

    // build the discrete upper envelope with u log u algorithm
    Discrete_upper_envelope due_ulogu;
    due_ulogu.build_ulogu(l, 5, 10);

    // build the discrete upper envelope via brute force
    Discrete_upper_envelope due_brute;
    due_brute.build_brute_force(l, 5, 10);

    // check with "known good result"
    EXPECT_EQ(2, due_brute.size());
    Envelope_iter i = due_brute.begin();
    EXPECT_EQ(*i, Envelope_cell(l4, 5,7)); ++i;
    EXPECT_EQ(*i, Envelope_cell(l6, 8,10)); ++i;
    EXPECT_TRUE(i == due_brute.end());

    // make sure the results match
    EXPECT_EQ(due_brute.size(), due_ulogu.size());
    Envelope_iter o = due_ulogu.begin();
    Envelope_iter b = due_brute.begin();
    while (o != due_ulogu.end()) {
        EXPECT_EQ(b->line, o->line);
        EXPECT_EQ(b->left, o->left);
        EXPECT_EQ(b->right, o->right);
        ++o;
        ++b;
    }
    EXPECT_TRUE(o == due_ulogu.end());
    EXPECT_TRUE(b == due_brute.end());
}




TEST(DiscreteUpperEnvelope, causedErrors02) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    // create lines
    std::vector< Line > l;
    l.push_back(Line(-1,  18,  6));
    l.push_back(Line(-5,  4,  11));
    l.push_back(Line( 5,  17, 17));
    l.push_back(Line(-3,  19, 18));

    // build the discrete upper envelope with u log u algorithm
    Discrete_upper_envelope due_ric;
    due_ric.build_ulogu(l, -100, 100);
    EXPECT_TRUE(due_ric.verify(l));
}


TEST(DiscreteUpperEnvelope, causedErrors03) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    // create lines
    // DLM: there exists a seed on this input that creates a permutation that
    // exposes a bug.
    DDAD_goodies::seed_rand();
    std::vector< Line > l;
    l.push_back(Line(   5,  5,  0 ));
    l.push_back(Line(   2,-20,  1 ));
    l.push_back(Line(   1,  9,  2 ));
    l.push_back(Line(  -5,  7,  3 ));
    l.push_back(Line(   3, -6,  4 ));
    l.push_back(Line(   2, -3,  5 ));
    l.push_back(Line(   1, 17,  6 ));
    l.push_back(Line(   4, -7,  7 ));
    l.push_back(Line(   4,  7,  8 ));
    l.push_back(Line(  -2, -3,  9 ));
    l.push_back(Line(  -2, 16,  10));
    l.push_back(Line(  -3,-17,  11));
    l.push_back(Line(   3, -3,  12));
    l.push_back(Line(   4,-14,  13));
    l.push_back(Line(   3, 16,  14));
    l.push_back(Line(  -4, -6,  15));
    l.push_back(Line(  -5, 16,  16));
    l.push_back(Line(  -3,  9,  17));
    l.push_back(Line(   2,-13,  18));
    l.push_back(Line(   3,-20,  19));

    // build the discrete upper envelope with u log u algorithm
    Discrete_upper_envelope due_ric;
    due_ric.build_ulogu(l, -100, 100);
    EXPECT_TRUE(due_ric.verify(l));
}


TEST(DiscreteUpperEnvelope, causedErrors04) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    // create lines
    std::vector< Line > l;
    l.push_back(Line( 2, 76, 9));
    l.push_back(Line( 4, 76, 3));

    // build the discrete upper envelope with u log u algorithm
    Discrete_upper_envelope due_orient;
    due_orient.build_orient(l, 1, 100);
    EXPECT_TRUE(due_orient.verify(l));
}


TEST(DiscreteUpperEnvelope, causedErrors05) {
    typedef Discrete_upper_envelope::Line Line;
    typedef Discrete_upper_envelope::Envelope_cell Envelope_cell;
    typedef Discrete_upper_envelope::const_iterator Envelope_iter;

    // create lines
    std::vector< Line > l;
    l.push_back(Line( 4, -4, 1));
    l.push_back(Line( 8, -28, 2));

    // build the discrete upper envelope with u log u algorithm
    Discrete_upper_envelope due_ric;

    int num_lines = l.size();
    Discrete_upper_envelope::Rand_order_mem ro(
            Discrete_upper_envelope::rand_order_mem_size(num_lines));
    Discrete_upper_envelope::Sorted_lines_mem so(
            Discrete_upper_envelope::sorted_lines_mem_size(num_lines));
    Discrete_upper_envelope::Building_envelope_mem be01(
            Discrete_upper_envelope::building_envelope_mem_size(num_lines));
    Discrete_upper_envelope::Building_envelope_mem be02(
            Discrete_upper_envelope::building_envelope_mem_size(num_lines));
    due_ric.build_ric_with_slope_unique_lines_sorted_by_slope(l, 5, ro, so, be01, be02);
    EXPECT_TRUE(due_ric.verify(l));
}


