/**
 * Filename: Shuffle.h
 * Project name: degDrivenAlgoDesign
 * Created on: 21 Jul 2011
 * @author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_SHUFFLE_H_
#define _DDAD_SHUFFLE_H_

#include <iterator>

#include "DDAD_base.h"

struct Shuffle {

    template <class Iterator>
    static void standard_sequential_sample(
            Iterator begin, Iterator end,
            Iterator sample, int num_samples) {

        int i = 1;
        int k = num_samples;
        int n = std::distance(begin, end);

        for (Iterator iter = begin ; iter != end ; ++iter, ++i) {
            if (DDAD_goodies::mrand() <= k / (n+1.-i)) {
                *sample++ = *iter;
                --k;
            }
        }
    }


    /** move num_samples from list2 to list1 */
    template < class List, class ListIter>
    static void standard_sequential_sample_with_removal(
            List& list1, ListIter insert_point,
            List& list2, int num_samples) {

        int i = 1;
        int k = num_samples;
        int n = list2.size();
        List tmp;

        ListIter iter = list2.begin();
        while (iter != list2.end() && k > 0 ) {
            if (DDAD_goodies::mrand() <= k / (n+1.-i)) {
                ListIter to_splice = iter;
                ++iter;
                tmp.splice(tmp.end(), list2, to_splice);
                --k;
            } else { ++iter; }
            ++i;
        }
        list1.splice(insert_point, tmp);
    }


    /** move num_samples from list to samples */
    template < typename InputIter, typename List >
    static void standard_sequential_sample_with_removal(
            InputIter samples, List& list, int num_samples) {

        int i = 1;
        int k = num_samples;
        int n = list.size();

        typename List::iterator li = list.begin();
        while (li != list.end() && k > 0 ) {
            if (DDAD_goodies::mrand() <= k / (n+1.-i)) {
                *samples++ = *li;
                li = list.erase(li);
                --k;
            } else { ++li; }
            ++i;
        }
    }
};

#endif
