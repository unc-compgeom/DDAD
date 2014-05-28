/**
 * Filename: Discrete_upper_envelope_drawer.h
 * Project name: degDrivenAlgoDesign
 * Created on: 02 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_DISCRETE_UPPER_ENVELOPE_DRAWER_H_
#define _DDAD_DISCRETE_UPPER_ENVELOPE_DRAWER_H_

#include "../base/Line_2.h"

#include "../algos/Discrete_upper_envelope.h"

#include "DDAD_drawer.h"

class Discrete_upper_envelope_drawer {

public:

    /**
     * Draw the discrete upper envelope
     * @param due the discrete upper envelope to draw
     */
    void static draw(const Discrete_upper_envelope& due,
            const DDAD_drawer::Color_map& map);

};

#endif
