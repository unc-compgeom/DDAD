/**
 * Filename: Radix_sort.h
 * Project name: RPVoronoi_deg2
 * Created on: 9/16/09
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_RADIX_SORT_H_
#define _DDAD_RADIX_SORT_H_

#include <vector>
#include <iterator>

static RLogChannel *log_radix_sort_pair
        = DEF_CHANNEL("debug/radix_sort_pair", Log_Info);

static RLogChannel *log_radix_sort = DEF_CHANNEL("debug/radix_sort", Log_Info);


struct Radix_sort {

    /**
     * Radix sort a set of key value pairs with key as
     * distinct positive integers
     */
    template <class Iterator, class DistroIter, class RevDistroIter>
    static void radix_sort_pair_helper(Iterator inBegin, Iterator inEnd,
            Iterator outBegin, DistroIter distroBegin,
            RevDistroIter distroRevBegin, RevDistroIter distroRevEnd,
            int mask, int n, DistroIter distroEnd, Iterator outEnd) {

        rLog(log_radix_sort_pair, "Input: \n%s",
                pair_vect_to_cstring(inBegin, inEnd));

        // set bin sizes to 0
        for (RevDistroIter cur = distroRevBegin ; cur != distroRevEnd ; ++cur) {
            *cur = 0;
        }

        // compute bin sizes
        for (Iterator cur = inBegin ; cur != inEnd ; ++cur) {
            ++(*(distroBegin + (cur->first / mask) % 10));
        }

        rLog(log_radix_sort_pair, "Distribution %s",
                thing_to_cstring(vect_to_string(distroBegin, distroEnd," ", " ")));
        rLog(log_radix_sort_pair, "Bin           0  1  2  3  4  5  6  7  8  9");

        // set bin idx
        int last = n;
        for (RevDistroIter cur = distroRevBegin ; cur != distroRevEnd ; ++cur) {
            last -= *cur;
            *cur = last;
        }

        rLog(log_radix_sort_pair, "Distribution %s",
                thing_to_cstring(vect_to_string(distroBegin, distroEnd," ", " ")));
        rLog(log_radix_sort_pair, "Bin           0  1  2  3  4  5  6  7  8  9");

        // shuffle
        int dIdx;
        for (Iterator cur = inBegin ; cur != inEnd ; ++cur) {
            dIdx = ((cur->first) / mask) % 10;
            *(outBegin + *(distroBegin + dIdx)) = *cur;
            ++(*(distroBegin + dIdx));
        }

        rLog(log_radix_sort, "Output:\n%s", pair_vect_to_cstring(outBegin, outEnd));
    }

    /**
     * Radix sort a set of key value pairs with key as distinct positive integers
     */
    template <class Iterator>
    static int radix_sort_pair(Iterator begin, Iterator end) {

        int mask = 1;
        int n = 0, max = 0;
        std::vector<int> distrib(10, 0);

        // first pass is to get the max elt and the number of elts
        // and construct aux storage
        for (Iterator cur = begin ; cur != end ; ++cur) {
            if (cur->first > max) { max = cur->first; }
            ++n;
        }
        typedef typename std::iterator_traits<Iterator>::value_type IterVal;
        std::vector< IterVal > tmp(n);

        while (mask <= max) {
            radix_sort_pair_helper(begin, end, tmp.begin(), distrib.begin(),
                    distrib.rbegin(), distrib.rend(), mask, n, distrib.end(),
                    tmp.end());
            mask *= 10;
            if (mask > max) { std::copy(tmp.begin(), tmp.end(), begin); }
            else {
                radix_sort_pair_helper(tmp.begin(), tmp.end(), begin,
                        distrib.begin(), distrib.rbegin(), distrib.rend(),
                        mask, n, distrib.end(), end);
                mask *= 10;
            }
        }
        return max;
    }


    template <class Iterator, class DistroIter, class RevDistroIter>
    static void radix_sort_helper(Iterator inBegin, Iterator inEnd,
            Iterator outBegin, DistroIter distroBegin,
            RevDistroIter distroRevBegin, RevDistroIter distroRevEnd,
            int mask, int n, DistroIter distroEnd, Iterator outEnd) {

        rLog(log_radix_sort, "Input:\n%s", vect_to_cstring(inBegin, inEnd));

        // set bin sizes to 0
        for (RevDistroIter cur = distroRevBegin ; cur != distroRevEnd ; ++cur) {
            *cur = 0;
        }

        // compute bin sizes
        for (Iterator cur = inBegin ; cur != inEnd ; ++cur) {
            ++(*(distroBegin + (*cur / mask) % 10));
        }

        rLog(log_radix_sort, "Distribution %s",
                thing_to_cstring(vect_to_string(distroBegin, distroEnd," ", " ")));
        rLog(log_radix_sort, "Bin           0  1  2  3  4  5  6  7  8  9");

        // set bin idx
        int last = n;
        for (RevDistroIter cur = distroRevBegin ; cur != distroRevEnd ; ++cur) {
            last -= *cur;
            *cur = last;
        }

        rLog(log_radix_sort, "Distribution %s",
                thing_to_cstring(vect_to_string(distroBegin, distroEnd," ", " ")));
        rLog(log_radix_sort, "Bin           0  1  2  3  4  5  6  7  8  9");

        // shuffle
        int dIdx;
        for (Iterator cur = inBegin ; cur != inEnd ; ++cur) {
            dIdx = (*cur / mask) % 10;
            *(outBegin + *(distroBegin + dIdx)) = *cur;
            ++(*(distroBegin + dIdx));
        }

        rLog(log_radix_sort, "Output:\n%s", vect_to_cstring(outBegin, outEnd));
    }

    /** Radix sort a set of positive integers */
    template <class Iterator>
    static int radix_sort(Iterator begin, Iterator end) {

        int mask = 1;
        int n = 0, max = 0;
        std::vector<int> distrib(10, 0);

        // first pass is to get the max elt and the number of elts
        // and construct aux storage
        for (Iterator cur = begin ; cur != end ; ++cur) {
            if (*cur > max) { max = *cur; }
            ++n;
        }
        rLog(log_radix_sort, "Max: %d", max);
        typedef typename std::iterator_traits<Iterator>::value_type IterVal;
        std::vector< IterVal > tmp(n);

        while (mask <= max) {
            radix_sort_helper(begin, end, tmp.begin(), distrib.begin(),
                    distrib.rbegin(), distrib.rend(), mask, n, distrib.end(),
                    tmp.end());
            mask *= 10;
            if (mask > max) { std::copy(tmp.begin(), tmp.end(), begin); }
            else {
                radix_sort_helper(tmp.begin(), tmp.end(), begin, distrib.begin(),
                        distrib.rbegin(), distrib.rend(), mask, n,
                        distrib.end(), end);
                mask *= 10;
            }
        }
        return max;
    }
};


#endif
