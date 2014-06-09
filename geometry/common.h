/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2012-12-30
 */

#ifndef GE_COMMON_H
#define GE_COMMON_H

#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4244)
#pragma warning(disable:4800)
#pragma warning(disable:4146)
#include "mpirxx.h"
#pragma warning(pop)
#pragma warning(disable:4100)

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <list>
#include <queue>
#include <limits>
#include <array>
#include <memory>
#include <cstdint>
#include <float.h>
#include <tuple>

// logging
#include "../logging/easylogging++.h"

#define UNUSED(x) (void)x;

namespace DDAD {

enum Orientation {
    ORIENTATION_LEFT,
    ORIENTATION_RIGHT,
    ORIENTATION_COLINEAR
};

enum Side {
    SIDE_NORTH,
    SIDE_SOUTH,
    SIDE_EAST,
    SIDE_WEST
};

enum Direction {
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_EAST,
    DIRECTION_WEST
};

enum Quadrant {
    QUADRANT_NORTHEAST,
    QUADRANT_NORTHWEST,
    QUADRANT_SOUTHEAST,
    QUADRANT_SOUTHWEST
};

enum Sign {
    SIGN_POSITIVE   =  1,
    SIGN_NEGATIVE   = -1,
    SIGN_ZERO       =  0
};

} // namespace DDAD

#endif // GE_COMMON_H
