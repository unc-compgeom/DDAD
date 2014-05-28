/**
 * Filename: Timer.h
 * Project name: degDrivenAlgoDesign
 * Created on: 17 Feb 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef OX_TIMER_UTIL_H
#define OX_TIMER_UTIL_H

//#include <ctime>
#include <sys/time.h>

/** A class of utilities for basic array manipulation */
class Timer {
protected:
    timeval t1, t2;

public:

    Timer() {}

    /** start the timer */
    void start() {
        gettimeofday(&t1, NULL);
    }

    /**
     * stop the timer
     * return the ellapsed time in seconds
     */
    double stop() {
        gettimeofday(&t2, NULL);
        return elapsed();
    }

    double stop_millisec() {
        gettimeofday(&t2, NULL);
        return elapsed_millisec();
    }

    /** return clocks per sec */
    //double clocks_per_sec() const { return CLOCKS_PER_SEC; }

    /** return the elapsed time in seconds */
    double elapsed() const { return elapsed_millisec()/1000.; }


    /** return the elapsed time rounded to the second*/
    double elapsed_sec() const {
        return (t2.tv_sec - t1.tv_sec);
    }

    /** return the elapsed time in miliseconds */
    double elapsed_millisec() const {
        double elapsed = (t2.tv_sec - t1.tv_sec) * 1000.0;
        elapsed += (t2.tv_usec - t1.tv_usec) / 1000.0;
        return elapsed;
    }
};

#endif

