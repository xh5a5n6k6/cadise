#pragma once

#include "math/tAabb3.h"

#include "fundamental/assertion.h"

#include <algorithm>
#include <limits>
#include <type_traits>

namespace cadise 
{

template<typename T>
inline TAABB3<T>::TAABB3() :
    TAABB3(
        TVector3<T>(std::numeric_limits<T>::max()), 
        TVector3<T>(std::numeric_limits<T>::min()))
{}

template<typename T>
inline TAABB3<T>::TAABB3(const TVector3<T>& vertex) :
    TAABB3(vertex, vertex)
{}

template<typename T>
inline TAABB3<T>::TAABB3(const TVector3<T>& minVertex, const TVector3<T>& maxVertex) :
    _minVertex(minVertex),
    _maxVertex(maxVertex) 
{}

template<typename T>
inline bool TAABB3<T>::isEmpty() const 
{
    return _minVertex.isEqualTo(TVector3<T>(std::numeric_limits<T>::max())) ||
           _maxVertex.isEqualTo(TVector3<T>(std::numeric_limits<T>::min()));
}

template<typename T>
inline bool TAABB3<T>::isInside(const TVector3<T>& position) const
{
    return _minVertex.isEqualTo(TVector3<T>::min(_minVertex, position)) &&
           _maxVertex.isEqualTo(TVector3<T>::max(_maxVertex, position));
}

template<typename T>
inline bool TAABB3<T>::isIntersectingAABB(
    const TVector3<T>& rayOrigin, 
    const TVector3<T>& rayRcpDirection, 
    const T            rayMinT, 
    const T            rayMaxT) const
{
    T localMinT;
    T localMaxT;

    return this->isIntersectingAABB(
        rayOrigin, 
        rayRcpDirection, 
        rayMinT, 
        rayMaxT,
        &localMinT,
        &localMaxT);
}

template<typename T>
inline bool TAABB3<T>::isIntersectingAABB(
    const TVector3<T>& rayOrigin,
    const TVector3<T>& rayRcpDirection,
    const T            rayMinT,
    const T            rayMaxT,
    T* const           out_boundMinT,
    T* const           out_boundMaxT) const
{
    CS_ASSERT(out_boundMinT);
    CS_ASSERT(out_boundMaxT);

    T minT = rayMinT;
    T maxT = rayMaxT;

    const TVector3<T> nearT = _minVertex.sub(rayOrigin).mul(rayRcpDirection);
    const TVector3<T> farT  = _maxVertex.sub(rayOrigin).mul(rayRcpDirection);

    // calculate x-slab interval
    if (rayRcpDirection.x() > static_cast<T>(0))
    {
        minT = std::max(minT, nearT.x());
        maxT = std::min(maxT, farT.x());
    }
    else
    {
        minT = std::max(minT, farT.x());
        maxT = std::min(maxT, nearT.x());
    }

    // calculate y-slab interval
    if (rayRcpDirection.y() > static_cast<T>(0)) 
    {
        minT = std::max(minT, nearT.y());
        maxT = std::min(maxT, farT.y());
    }
    else
    {
        minT = std::max(minT, farT.y());
        maxT = std::min(maxT, nearT.y());
    }

    // calculate z-slab interval
    if (rayRcpDirection.z() > static_cast<T>(0))
    {
        minT = std::max(minT, nearT.z());
        maxT = std::min(maxT, farT.z());
    }
    else 
    {
        minT = std::max(minT, farT.z());
        maxT = std::min(maxT, nearT.z());
    }

    // check if intersection exists
    if (minT > maxT)
    {
        return false;
    }

    *out_boundMinT = minT;
    *out_boundMaxT = maxT;

    return true;
}

template<typename T>
inline void TAABB3<T>::reset() 
{
    this->setMinVertex(TVector3<T>(std::numeric_limits<T>::max()));
    this->setMaxVertex(TVector3<T>(std::numeric_limits<T>::min()));
}

template<typename T>
inline T TAABB3<T>::surfaceArea() const
{
    return static_cast<T>(2) * this->halfSurfaceArea();
}

template<typename T>
inline T TAABB3<T>::halfSurfaceArea() const
{
    const TVector3<T> extent = this->extent();

    return extent.x() * extent.y() +
           extent.x() * extent.z() +
           extent.y() * extent.z();
}

template<typename T>
inline TVector3<T> TAABB3<T>::extent() const 
{
    return _maxVertex.sub(_minVertex);
}

template<typename T>
inline TVector3<T> TAABB3<T>::centroid() const
{
    if constexpr (std::is_integral_v<T>) 
    {
        return _minVertex.add(_maxVertex).div(static_cast<T>(2));
    }
    else 
    { 
        return _minVertex.add(_maxVertex).mul(static_cast<T>(0.5_r)); 
    }
}

template<typename T>
inline constant::AxisType TAABB3<T>::maxAxis() const 
{
    return this->extent().maxDimension();
}

template<typename T>
inline TAABB3<T> TAABB3<T>::unionWith(const TAABB3<T>& rhs) const 
{
    return TAABB3<T>(
        TVector3<T>::min(_minVertex, rhs._minVertex),
        TVector3<T>::max(_maxVertex, rhs._maxVertex));
}

template<typename T>
inline TAABB3<T>& TAABB3<T>::unionWithLocal(const TAABB3<T>& rhs) 
{
    _minVertex = TVector3<T>::min(_minVertex, rhs._minVertex);
    _maxVertex = TVector3<T>::max(_maxVertex, rhs._maxVertex);

    return *this;
}

template<typename T>
inline TAABB3<T> TAABB3<T>::unionWith(const TVector3<T>& rhs) const
{
    return TAABB3<T>(
        TVector3<T>::min(_minVertex, rhs),
        TVector3<T>::max(_maxVertex, rhs));
}

template<typename T>
inline TAABB3<T>& TAABB3<T>::unionWithLocal(const TVector3<T>& rhs)
{
    _minVertex = TVector3<T>::min(_minVertex, rhs);
    _maxVertex = TVector3<T>::max(_maxVertex, rhs);
    
    return *this;
}

template<typename T>
inline TAABB3<T> TAABB3<T>::expand(const T rhs) const
{
    return TAABB3<T>(
        _minVertex.sub(rhs),
        _maxVertex.add(rhs));
}

template<typename T>
inline TAABB3<T>& TAABB3<T>::expandLocal(const T rhs)
{
    _minVertex.subLocal(rhs);
    _maxVertex.addLocal(rhs);

    return *this;
}

template<typename T>
inline const TVector3<T>& TAABB3<T>::minVertex() const
{
    return _minVertex;
}

template<typename T>
inline const TVector3<T>& TAABB3<T>::maxVertex() const
{
    return _maxVertex;
}

template<typename T>
inline void TAABB3<T>::setMinVertex(const TVector3<T>& minVertex)
{
    _minVertex = minVertex;
}

template<typename T>
inline void TAABB3<T>::setMaxVertex(const TVector3<T>& maxVertex)
{
    _maxVertex = maxVertex;
}

template<typename T>
inline void TAABB3<T>::setMinMaxVertices(
    const TVector3<T>& minVertex,
    const TVector3<T>& maxVertex)
{
    this->setMinVertex(minVertex);
    this->setMaxVertex(maxVertex);
}

template<typename T>
inline void TAABB3<T>::set(const TAABB3<T>& other)
{
    _minVertex = other._minVertex;
    _maxVertex = other._maxVertex;
}

} // namespace cadise