#pragma once

#include "math/type/fundamentalType.h"

namespace cadise 
{

///////////////////////////////////////////////////////////
// 2-dimensional vector with arbitrary type
template<typename T>
class TVector2;

using Vector2I = TVector2<int32>;
using Vector2R = TVector2<real>;
using Vector2S = TVector2<std::size_t>;
using Vector2D = TVector2<float64>;


///////////////////////////////////////////////////////////
// 3-dimensional vector with arbitrary type
template<typename T>
class TVector3;

using Vector3I = TVector3<int32>;
using Vector3R = TVector3<real>;
using Vector3S = TVector3<std::size_t>;
using Vector3D = TVector3<float64>;


///////////////////////////////////////////////////////////
// 4x4 matrix with arbitrary type
template<typename T>
class TMatrix4;

using Matrix4R = TMatrix4<real>;
using Matrix4D = TMatrix4<float64>;


///////////////////////////////////////////////////////////
// 2-dimensional axis-aligned bounding box with arbitrary type 
template<typename T>
class TAABB2;

using AABB2I = TAABB2<int32>;
using AABB2R = TAABB2<real>;
using AABB2S = TAABB2<std::size_t>;
using AABB2D = TAABB2<float64>;


///////////////////////////////////////////////////////////
// 3-dimensional axis-aligned bounding box with arbitrary type 
template<typename T>
class TAABB3;

using AABB3I = TAABB3<int32>;
using AABB3R = TAABB3<real>;
using AABB3S = TAABB3<std::size_t>;
using AABB3D = TAABB3<float64>;


///////////////////////////////////////////////////////////
// three-dimensional local coordinate system with 
// arbitrary type
template<typename T>
class TLocalCoordinateSystem3;

using LCS3R = TLocalCoordinateSystem3<real>;
using LCS3D = TLocalCoordinateSystem3<float64>;


///////////////////////////////////////////////////////////
template<typename T, std::size_t N>
class TArithmeticArray;

} // namespace cadise