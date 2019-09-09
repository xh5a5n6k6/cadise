#pragma once

#include "math/fundamentalType.h"

namespace cadise {

// arbitrary dimension vector
//
template<typename T, uint32 Size>
class Vector;

using Vector2I = Vector<int32, 2>;
using Vector2R = Vector<real, 2>;
using Vector3R = Vector<real, 3>;

// arbitrary dimension axis-aligned bounding box
//
template<typename T, uint32 Size>
class AABB;

using AABB2I = AABB<int32, 2>;
using AABB3R = AABB<real, 3>;

} // namespace cadise