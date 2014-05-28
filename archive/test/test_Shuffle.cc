/**
 * Filename: test_Shuffle.cc
 * Project name: degDrivenAlgoDesign
 * Created on: 21 Jul 2011
 * @author: David L. Millman <dave@cs.unc.edu>
 */

#include <vector>
#include <list>

#include "../base/DDAD_rlog.h"
#include "../base/Shuffle.h"

#include <gtest/gtest.h>

TEST(Shuffle, standardSequentialSample) {
    int n = 10, k = 5;
    DDAD_goodies::seed_rand(1);

    std::vector<int> v, shuffle(k);

    for (int i = 0 ; i < n ; ++i) { v.push_back(i); }
    Shuffle::standard_sequential_sample(
            v.begin(), v.end(), shuffle.begin(), k);

    EXPECT_EQ((uint)5,shuffle.size());
    EXPECT_EQ(1,shuffle[0]);
    EXPECT_EQ(5,shuffle[1]);
    EXPECT_EQ(6,shuffle[2]);
    EXPECT_EQ(8,shuffle[3]);
    EXPECT_EQ(9,shuffle[4]);

}

TEST(Shuffle, standardSequentialSampleWithRemoval) {
    int n = 10, k1 = 5, k2 = 2;
    DDAD_goodies::seed_rand(1);

    std::list<int> v, shuffle;
    typedef std::list<int>::iterator list_iter;

    for (int i = 0 ; i < n ; ++i) { v.push_back(i); }
    Shuffle::standard_sequential_sample_with_removal(shuffle, shuffle.end(), v, k1);

    EXPECT_EQ((uint)k1,shuffle.size());
    list_iter i = shuffle.begin();
    EXPECT_EQ(1,*i); ++i;
    EXPECT_EQ(5,*i); ++i;
    EXPECT_EQ(6,*i); ++i;
    EXPECT_EQ(8,*i); ++i;
    EXPECT_EQ(9,*i); ++i;
    EXPECT_TRUE(shuffle.end() == i);

    EXPECT_EQ((uint)5,v.size());
    i = v.begin();
    EXPECT_EQ(0,*i); ++i;
    EXPECT_EQ(2,*i); ++i;
    EXPECT_EQ(3,*i); ++i;
    EXPECT_EQ(4,*i); ++i;
    EXPECT_EQ(7,*i); ++i;
    EXPECT_TRUE(v.end() == i);

    Shuffle::standard_sequential_sample_with_removal(shuffle, shuffle.begin(), v, 2);
    EXPECT_EQ((uint)k1+k2, shuffle.size());
    i = shuffle.begin();
    EXPECT_EQ(3,*i); ++i;
    EXPECT_EQ(7,*i); ++i;
    EXPECT_EQ(1,*i); ++i;
    EXPECT_EQ(5,*i); ++i;
    EXPECT_EQ(6,*i); ++i;
    EXPECT_EQ(8,*i); ++i;
    EXPECT_EQ(9,*i); ++i;

    EXPECT_EQ((uint)3,v.size());
    i = v.begin();
    EXPECT_EQ(0,*i); ++i;
    EXPECT_EQ(2,*i); ++i;
    EXPECT_EQ(4,*i); ++i;
    EXPECT_TRUE(v.end() == i);

}


TEST(Shuffle, standardSequentialSampleWithRemovalVect) {
    int n = 10, k1 = 5, k2 = 2;
    DDAD_goodies::seed_rand(1);

    std::list<int> v;
    std::vector<int> shuffle(n, -1);

    typedef std::list<int>::iterator list_iter;
    typedef std::vector<int>::iterator vect_iter;
    for (int i = 0 ; i < n ; ++i) { v.push_back(i); }

    // first batch of size k1
    Shuffle::standard_sequential_sample_with_removal(shuffle.begin(), v, k1);
    EXPECT_EQ((uint)k1,v.size());
    list_iter j = v.begin();
    EXPECT_EQ(0,*j); ++j;
    EXPECT_EQ(2,*j); ++j;
    EXPECT_EQ(3,*j); ++j;
    EXPECT_EQ(4,*j); ++j;
    EXPECT_EQ(7,*j); ++j;
    EXPECT_TRUE(v.end() == j);

    EXPECT_EQ((uint)n, shuffle.size());
    vect_iter i = shuffle.begin();
    EXPECT_EQ(1,*i); ++i;
    EXPECT_EQ(5,*i); ++i;
    EXPECT_EQ(6,*i); ++i;
    EXPECT_EQ(8,*i); ++i;
    EXPECT_EQ(9,*i); ++i;
    EXPECT_EQ(-1,*i); ++i;
    EXPECT_EQ(-1,*i); ++i;
    EXPECT_EQ(-1,*i); ++i;
    EXPECT_EQ(-1,*i); ++i;
    EXPECT_EQ(-1,*i); ++i;
    EXPECT_TRUE(shuffle.end() == i);

    // first batch of size k2
    Shuffle::standard_sequential_sample_with_removal(shuffle.begin()+k1, v, k2);
    EXPECT_EQ((uint)3,v.size());
    j = v.begin();
    EXPECT_EQ(0,*j); ++j;
    EXPECT_EQ(2,*j); ++j;
    EXPECT_EQ(4,*j); ++j;
    EXPECT_TRUE(v.end() == j);

    EXPECT_EQ((uint)n, shuffle.size());
    i = shuffle.begin();
    EXPECT_EQ(1,*i); ++i;
    EXPECT_EQ(5,*i); ++i;
    EXPECT_EQ(6,*i); ++i;
    EXPECT_EQ(8,*i); ++i;
    EXPECT_EQ(9,*i); ++i;
    EXPECT_EQ(3,*i); ++i;
    EXPECT_EQ(7,*i); ++i;
    EXPECT_EQ(-1,*i); ++i;
    EXPECT_EQ(-1,*i); ++i;
    EXPECT_EQ(-1,*i); ++i;
    EXPECT_TRUE(shuffle.end() == i);

    // third batch of size n
    Shuffle::standard_sequential_sample_with_removal(shuffle.begin()+k1+k2, v, n);
    EXPECT_EQ((uint)0,v.size());
    j = v.begin();
    EXPECT_TRUE(v.end() == j);

    EXPECT_EQ((uint)n, shuffle.size());
    i = shuffle.begin();
    EXPECT_EQ(1,*i); ++i;
    EXPECT_EQ(5,*i); ++i;
    EXPECT_EQ(6,*i); ++i;
    EXPECT_EQ(8,*i); ++i;
    EXPECT_EQ(9,*i); ++i;
    EXPECT_EQ(3,*i); ++i;
    EXPECT_EQ(7,*i); ++i;
    EXPECT_EQ(0,*i); ++i;
    EXPECT_EQ(2,*i); ++i;
    EXPECT_EQ(4,*i); ++i;
    EXPECT_TRUE(shuffle.end() == i);
}


