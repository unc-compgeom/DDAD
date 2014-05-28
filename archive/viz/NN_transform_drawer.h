/**
 * Filename: NN_transform_drawer.h
 * Project name: degDrivenAlgoDesign
 * Created on: 07 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */


#ifndef _DDAD_NN_TRANSFORM_DRAWER_H_
#define _DDAD_NN_TRANSFORM_DRAWER_H_

#include "../base/Point_2.h"
#include "../base/Constructions_2.h"

#include "../algos/NN_transform.h"

#include "DDAD_drawer.h"

class NN_transform_drawer {
protected:
    typedef Point_2::Std_point Point;
    typedef DDAD_drawer::Color Color;
    typedef DDAD_drawer::Color_map Color_map;
    typedef NN_transform::Site_iterator Site_iterator;

protected:

    /**
     * Draw the colored grid representation of the nearest neighbor transform
     * @param nn the nearest neighborm transform
     * @param color_map specifies the color of each site
     */
    void static draw_grid(const NN_transform& nn, const Color_map& color_map);

    /**
     * Draw the sites that generated the nearest neighborm transform
     * @param nn the nearest neighbor transform
     * @param color_map specifies the color of each site
     */
    void static draw_sites(const NN_transform& dv, const Color_map& color_map);

public:

    /**
     * Draw the nearest neighbor transform.
     * @param nn the nearest neighbor transform
     * @param color_map specifies the color of each site
     */
    void static draw(const NN_transform& nn, const Color_map& color_map);
};

#endif
