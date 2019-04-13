#pragma once

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <memory>
#include <limits>
#include <vector>
#include <string>
#include <algorithm>

namespace cadise {

inline constexpr float CADISE_RAY_EPSILON 
	= 0.01f;

inline constexpr float CADISE_PI 
	= 3.1415926535897932f;

inline constexpr float CADISE_INV_PI 
	= 0.3183098861837906f;

inline constexpr float CADISE_INV_EIGHT_PI 
	= 0.0397887357729738f;

} // namespace cadise