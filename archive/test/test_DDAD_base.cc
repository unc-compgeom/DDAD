/**
 * Filename: test_DDAD_base.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 11 Nov 2010
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "../base/DDAD_base.h"

#include <gtest/gtest.h>

TEST(DDADBase, Sign) {
    DDAD::Sign p = DDAD::POSITIVE;
    DDAD::Sign n = DDAD::NEGATIVE;
    DDAD::Sign z = DDAD::ZERO;
    EXPECT_EQ("Positive", DDAD::to_string(p));
    EXPECT_EQ("Negative", DDAD::to_string(n));
    EXPECT_EQ("Zero", DDAD::to_string(z));
}

TEST(DDADBase, RLog) {

#ifdef DDAD_LOGGING_ON
    EXPECT_EQ("Positive", thing_to_string(DDAD::POSITIVE));
#else
    EXPECT_EQ("", thing_to_string(DDAD::POSITIVE));
#endif
}

struct Point { int x,y,id; Point(int xi=0,int yi=0,int idi=-1) : x(xi), y(yi), id(idi) {} };
TEST(DDADBase, GoodiesSiteGen) {
    std::vector< Point > v;
    DDAD_goodies::create_random_unique(std::back_inserter(v), Point(), 5, 10);
    for (size_t i = 0 ; i < v.size() ; ++i) {
        EXPECT_GE(v[i].id, 0);
        for (size_t j = i+1 ; j < v.size() ; ++j) {
            EXPECT_FALSE(v[i].x == v[j].x && v[i].y == v[j].y);
            EXPECT_LT(v[i].id, v[j].id);
        }
    }
}

