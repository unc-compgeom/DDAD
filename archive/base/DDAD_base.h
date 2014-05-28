/**
 * Filename: DDAD_base.h
 * Project name: degDrivenAlgoDesign
 * Created on: 11 Nov 2010
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_DDAD_BASE_H_
#define _DDAD_DDAD_BASE_H_

#include <string>
#include <sstream>
#include <exception>
#include <cmath>
#include <vector>
#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "DDAD_rlog.h"

struct DDAD {

    //========================================================================
    //                              ERRORS
    //========================================================================

    /** A basic error in which all other error derrive from */
    class Error : public std::exception {
        virtual const char* what() const throw() {
            return "A basic error occurred";
        }
    };

    /**
     * The case not covered error should be thown when an algorithm falls into a state
     * that we not described by the implementation.
     */
    class Case_not_covered_error : Error {
        virtual const char* what() const throw() {
            return "A case not handeled in the implementation occurred";
        }
    };

    /**
     * Unexpected state error should be thrown when a function ends up
     * in some state that makes no sense
     */
    class Unexpected_state_error : Error {
        virtual const char* what() const throw() {
            return "Some unexpected state occured";
        }
    };

    /**
     * The unitialized error should be thown when an value is sent to
     * a print function but it has not been initalized
     */
    class Print_uninitalized_value_error : Error {
        virtual const char* what() const throw() {
            return "The input to a function did not meet a precondition";
        }
    };



    //==============================================================================
    //                              SIGN Type
    //==============================================================================

    /**
     * A trivarite type. Its main intent is as the result for the evaluation of
     * polynomials and predicates.
     */
    enum Sign { POSITIVE=1, NEGATIVE=-1, ZERO=0 };

    /**
     * Return the string representation of the sign
     * @param s the sign to convet to s string
     */
    static std::string to_string(const Sign& s);

    /**
     * Ouput operator for sign
     * @param s the sgn to output
     */
    friend std::ostream& operator<<(std::ostream& os, const Sign& s);

    /**
     * Return the sign of a value
     * @param i the value to check the sign of
     * @return the sign of i
     */
    template < typename NumberType >
    static Sign sign(NumberType i) {
        if (i < 0) { return NEGATIVE; }
        else if (i > 0) { return POSITIVE; }
        else { return ZERO; }
    }
};

//==============================================================================
//          Small functions to help with prototyping and testing
//==============================================================================
struct DDAD_goodies {

    static void seed_rand(int i=-1) {
        time_t t;
        if (i < 0) {
	    t = time(NULL);
            rWarning("Seed: %d",(int)t);
        } else {
            t = i;
        }
        srand(t);
    }
    static float mrand() { return ((double)rand()/(double)RAND_MAX); }
    static int int_coord_rand(int k, bool positiveOnly = true) {
        return ((!positiveOnly && mrand() < .5)?-1:1)
                * (int)(ceil((k) * mrand()));
    }

    /** push onto iter a set of n rand pairs of type Tw/ coord vals in 1:U */
    template <class InsertIter, class T>
    static void create_random_unique(InsertIter iter, const T&,
            int n, int U1, int U2=-1, bool first_coord_positive_only=true,
            bool second_coord_positive_only=true) {
        typedef std::pair<int, int> IntPair;
        typedef std::vector< IntPair > IntPairVect;
        typedef typename InsertIter::value_type Coord_pair;

        // set U2 if it is the default
        if (U2 == -1) { U2 = U1; }

        // verify input
        rAssert((n >= 0 && U1 > 1 && U2 > 1));

        // fill the coordinates with unique pairs
        IntPairVect coords(n);
        for (int i = 0 ; i < n ; ++i) {
            bool isDup;
            IntPair candidate;
            do {
                isDup = false;
                candidate = IntPair(
                        int_coord_rand(U1, first_coord_positive_only),
                        int_coord_rand(U2, second_coord_positive_only));
                for (int j = 0 ; j < i && !isDup ; ++j) {
                    isDup = candidate == coords[j];
                }
            } while (isDup);
            coords[i] = candidate;
        }

        // now coords is unique points push them as sites
        int idx = 0;
        for (IntPairVect::iterator c_i = coords.begin() ;
                c_i != coords.end() ; ++c_i, ++idx) {
            *iter++ = T(c_i->first, c_i->second, idx);
        }
    }


    /** push onto iter a set of n rand pairs of type Tw/ coord vals in 1:U */
    template <class InsertIter, class T>
    static void create_random_unique_percent(InsertIter iter, const T&,
            float percent, int U) {

        // verify input
        rAssert(0 < percent && percent < 1 && U > 1);
        int idx = 0;
        for (int i = 1 ; i <= U ; ++i) {
            for (int j = 1 ; j <= U ; ++j) {
                if (mrand() <= percent) { *iter++ = T(i,j,idx); ++idx; }
            }
        }
    }


};

struct DDAD_util {

    template <class Iterator>
    static Iterator next(Iterator i) {
        Iterator tmp = i;
        return ++tmp;
    }

    template <class Iterator>
    static Iterator prev(Iterator i) {
        Iterator tmp = i;
        return --tmp;
    }

    template <class Iterator>
    static void inc_safe_advance(Iterator& iter, int dist, Iterator end) {
        for (int i = 0 ; i < dist && iter != end ; ++i) { ++iter; }
    }


    template <class Int>
    static int log2 (Int val) {
        int ret = -1;
        while (val != 0) { val >>= 1; ret++; }
        return ret;
    }

    template <class Iterator>
    static std::string vect_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) {
        std::ostringstream os;
        int i = 0;
        for (Iterator iter = begin ; iter != end ; ++iter, ++i) {
            os << pre;
            if (count) { os << i << ".."; }
            os << *iter << post;
        }
        return os.str();
    }

    template <typename Iterator>
    static std::string vect_of_ptrs_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) {
        std::ostringstream os;
        int i = 0;
        for (Iterator iter = begin ; iter != end ; ++iter, ++i) {
            os << pre;
            if (count) { os << i << ".."; }
            os << **iter << post;
        }
        return os.str();
    }
};

#endif
