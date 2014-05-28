/**
 * Filename: test_DDAD_base.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 24 Nov 2010
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include <algorithm>
#include "../base/Radix_sort.h"

#include <gtest/gtest.h>

TEST(RadixSort, radixSort) {
    std::vector<int> v;
    for (int i = 0 ; i <= 1100 ; i+=10) { v.push_back(i); }
    std::random_shuffle(v.begin(), v.end());
    Radix_sort::radix_sort(v.begin(), v.end());
    for (size_t i = 1 ; i < v.size() ; ++i) { EXPECT_LT(v[i-1], v[i]); }

    // check when the larges value is on a radix boundary (a gotcha bug)
    std::vector<int> v2;
    v2.push_back(6);
    v2.push_back(26);
    v2.push_back(35);
    v2.push_back(43);
    v2.push_back(45);
    v2.push_back(25);
    v2.push_back(100);
    v2.push_back(15);
    v2.push_back(20);
    Radix_sort::radix_sort(v2.begin()+3, v2.begin()+7);
    for (size_t i = 4 ; i < 7 ; ++i) { EXPECT_LT(v2[i-1], v2[i]); }
}

TEST(RadixSort, radixSortPair) {
    typedef std::pair<int, int> IntPair;
    std::vector< IntPair > v;
    for (int i = 0 ; i <= 1100 ; i+=10) { v.push_back( IntPair(i,i) ); }
    std::random_shuffle(v.begin(), v.end());
    for (size_t i = 0 ; i < v.size() ; ++i) { v[i].second = i; }
    std::vector< IntPair > v_cpy = v;
    Radix_sort::radix_sort_pair(v.begin(), v.end());

    // post ordering
    for (size_t i = 1 ; i < v.size() ; ++i) { EXPECT_LT(v[i-1].first, v[i].first); }

    // check key value pairs don't get mixed up
    for (size_t i = 0 ; i < v.size() ; ++i) {
        EXPECT_EQ(v[i].first, v_cpy[ v[i].second ].first);
    }

    // check when largest value is on radix boundary (a gotcha bug)
    std::vector< IntPair > v2;
    v2.push_back(IntPair(6,0));
    v2.push_back(IntPair(26,1));
    v2.push_back(IntPair(35,2));
    v2.push_back(IntPair(43,3));
    v2.push_back(IntPair(45,4));
    v2.push_back(IntPair(25,5));
    v2.push_back(IntPair(100,6));
    v2.push_back(IntPair(15,7));
    v2.push_back(IntPair(20,8));
    Radix_sort::radix_sort_pair(v2.begin()+3, v2.begin()+7);
    for (size_t i = 4 ; i < 7 ; ++i) { EXPECT_LT(v2[i-1].first, v2[i].first); }

}



