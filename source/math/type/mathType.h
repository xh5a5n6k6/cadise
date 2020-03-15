#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

// arbitrary dimension vector
template<typename T, std::size_t N>
class Vector;

using Vector2I = Vector<int32, 2>;
using Vector2R = Vector<real, 2>;
using Vector2S = Vector<std::size_t, 2>;
using Vector3R = Vector<real, 3>;

// arbitrary type 4x4 matrix
template<typename T>
class Matrix4;

using Matrix4R = Matrix4<real>;

// arbitrary dimension axis-aligned bounding box
template<typename T, std::size_t N>
class AABB;

using AABB2I = AABB<int32, 2>;
using AABB3R = AABB<real, 3>;

} // namespace cadise