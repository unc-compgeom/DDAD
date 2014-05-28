/**
 * Filename: Constructions_2.h
 * Project name: degDrivenAlgoDesign
 * Created on: 18 Feb 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */


#ifndef _CONSTRUCTIONS_2_H_
#define _CONSTRUCTIONS_2_H_

#include "DDAD_base.h"

#include "Point_2.h"
#include "Line_2.h"


class Constructions_2 {

public:
    /**
     * Return the x and y Cartesian coordinates of the point
     *
     * @param x referance to return x coordinate value
     * @param y referance to return y coordinate value
     * @param p point to return the coordinates.
     */
    static void coords(float &x, float &y, const Point_2::Std_point& p);


    /**
     * A standard point's coordinates can be reported exactly as an integer
     * returnt the integer representation of the x-coordinate of p
     *
     * @param the point to return the x coordinate of
     * @return the x-coordinate of p as an integer
     */
    static int coord_x(const Point_2::Std_point& p);

    /**
     * A standard point's coordinates can be reported exactly as an integer
     * returnt the integer representation of the y-coordinate of p
     *
     * @param the point to return the y coordinate of
     * @return the y-coordinate of p as an integer
     */
    static int coord_y(const Point_2::Std_point& p);

    /**
     * Given two points p and q, return y-coordinates of
     * the midpoint of p and q.
     *
     * @param p input point
     * @param q input point
     * @return the y-coodinate of mid point of p and q with p and
     */
    static float coord_y_mid_point(const Point_2::Std_point& p,
            const Point_2::Std_point& q);

    /**
     * Given two points p=(px,py) and q=(qx,qy) return the euclidean
     * distance between p and q.
     *
     * @param px x coord of p
     * @param py y coord of p
     * @param qx x coord of q
     * @param qy y coord of q
     * @return euclidean distance between p and q
     */
    static float dist(float px, float py, float qx, float qy);
};

#endif
