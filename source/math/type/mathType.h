#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

// vector with arbitrary type and dimension
template<typename T, std::size_t N>
class Vector;

using Vector2I = Vector<int32, 2>;
using Vector3I = Vector<int32, 3>;
using Vector4I = Vector<int32, 4>;
using Vector2R = Vector<real, 2>;
using Vector3R = Vector<real, 3>;
using Vector4R = Vector<real, 4>;
using Vector2S = Vector<std::size_t, 2>;
using Vector3S = Vector<std::size_t, 3>;
using Vector4S = Vector<std::size_t, 4>;

// 4x4 matrix with arbitrary type
template<typename T>
class Matrix4;

using Matrix4R = Matrix4<real>;

// axis-aligned bounding box with arbitrary type and dimension
template<typename T, std::size_t N>
class AABB;

using AABB2I = AABB<int32, 2>;
using AABB3I = AABB<int32, 3>;
using AABB2R = AABB<real, 2>;
using AABB3R = AABB<real, 3>;
using AABB2S = AABB<std::size_t, 2>;
using AABB3S = AABB<std::size_t, 3>;

} // namespace cadise