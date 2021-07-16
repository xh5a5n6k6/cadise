#pragma once

#include "math/tVector2.h"

namespace cadise {

// AABB is for axis-aligned bounding box
template<typename T>
class TAABB2 {
public:
    TAABB2();
    explicit TAABB2(const TVector2<T>& vertex);
    TAABB2(const TVector2<T>& minVertex, const TVector2<T>& maxVertex);

    bool isEmpty() const;

    T area() const;
    TVector2<T> extent() const;
    constant::AxisType maxAxis() const;

    const TVector2<T>& minVertex() const;
    const TVector2<T>& maxVertex() const;

    void setMinVertex(const TVector2<T>& minVertex);
    void setMaxVertex(const TVector2<T>& maxVertex);
    void setMinMaxVertices(
        const TVector2<T>& minVertex,
        const TVector2<T>& maxVertex);

    void set(const TAABB2& other);

private:
    TVector2<T> _minVertex;
    TVector2<T> _maxVertex;
};

} // namespace cadise

#include "math/tAabb2.ipp"