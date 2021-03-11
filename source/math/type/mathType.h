#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

///////////////////////////////////////////////////////////
// vector with arbitrary type and dimension
template<typename T, std::size_t N>
class TVector;

using Vector2I = TVector<int32, 2>;
using Vector3I = TVector<int32, 3>;
using Vector4I = TVector<int32, 4>;
using Vector2R = TVector<real, 2>;
using Vector3R = TVector<real, 3>;
using Vector4R = TVector<real, 4>;
using Vector2S = TVector<std::size_t, 2>;
using Vector3S = TVector<std::size_t, 3>;
using Vector4S = TVector<std::size_t, 4>;
using Vector2D = TVector<float64, 2>;
using Vector3D = TVector<float64, 3>;
using Vector4D = TVector<float64, 4>;

///////////////////////////////////////////////////////////
// 4x4 matrix with arbitrary type
template<typename T>
class TMatrix4;

using Matrix4R = TMatrix4<real>;
using Matrix4D = TMatrix4<float64>;

///////////////////////////////////////////////////////////
// axis-aligned bounding box with arbitrary type 
// and dimension
template<typename T, std::size_t N>
class TAABB;

using AABB2I = TAABB<int32, 2>;
using AABB3I = TAABB<int32, 3>;
using AABB2R = TAABB<real, 2>;
using AABB3R = TAABB<real, 3>;
using AABB2S = TAABB<std::size_t, 2>;
using AABB3S = TAABB<std::size_t, 3>;
using AABB2D = TAABB<float64, 2>;
using AABB3D = TAABB<float64, 3>;


///////////////////////////////////////////////////////////
// three-dimensional local coordinate system with 
// arbitrary type
template<typename T>
class TLocalCoordinateSystem3;

using LCS3R = TLocalCoordinateSystem3<real>;
using LCS3D = TLocalCoordinateSystem3<float64>;

} // namespace cadise