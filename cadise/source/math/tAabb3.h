#pragma once

#include "math/tVector3.h"

namespace cadise {

// AABB is for axis-aligned bounding box
template<typename T>
class TAABB3 {
public:
    TAABB3();
    explicit TAABB3(const TVector3<T>& vertex);
    TAABB3(const TVector3<T>& minVertex, const TVector3<T>& maxVertex);

    bool isEmpty() const;
    bool isInside(const TVector3<T>& position) const;

    bool isIntersectingAABB(
        const TVector3<T>& rayOrigin, 
        const TVector3<T>& rayRcpDirection, 
        const T            rayMinT, 
        const T            rayMaxT) const;

    bool isIntersectingAABB(
        const TVector3<T>& rayOrigin,
        const TVector3<T>& rayRcpDirection,
        const T            rayMinT,
        const T            rayMaxT,
        T* const           out_boundMinT,
        T* const           out_boundMaxT) const;

    void reset();
    T surfaceArea() const;
    T halfSurfaceArea() const;
    TVector3<T> extent() const;
    TVector3<T> centroid() const;
    constant::AxisType maxAxis() const;

    TAABB3  unionWith(const TAABB3& rhs) const;
    TAABB3& unionWithLocal(const TAABB3& rhs);
    TAABB3  unionWith(const TVector3<T>& rhs) const;
    TAABB3& unionWithLocal(const TVector3<T>& rhs);

    TAABB3  expand(const T rhs) const;
    TAABB3& expandLocal(const T rhs);

    const TVector3<T>& minVertex() const;
    const TVector3<T>& maxVertex() const;

    void setMinVertex(const TVector3<T>& minVertex);
    void setMaxVertex(const TVector3<T>& maxVertex);
    void setMinMaxVertices(
        const TVector3<T>& minVertex,
        const TVector3<T>& maxVertex);

    void set(const TAABB3& other);

private:
    TVector3<T> _minVertex;
    TVector3<T> _maxVertex;
};

} // namespace cadise

#include "math/tAabb3.ipp"