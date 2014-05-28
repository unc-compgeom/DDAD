/**
 * Filename: Convex_hull_drawer.h
 * Project name: degDrivenAlgoDesign
 * Created on: 26 May 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_CONVEX_HULL_DRAWER_
#define _DDAD_CONVEX_HULL_DRAWER_

#include "../base/Point_2.h"

#include "../algos/Convex_hull.h"

#include "DDAD_drawer.h"

class Convex_hull_drawer {

public:

    /**
     * Draw the convex hull computed via graham yao
     * @param ch the convex hull to draw
     */
    void static draw(const Convex_hull& ch);
};

#endif
