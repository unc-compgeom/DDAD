/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
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
#include <limits>
#include <array>
#include <memory>
#include <cstdint>
#include <float.h>
#include <tuple>

// logging
#define NOMINMAX
#include "../utility/easylogging++.h"

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
