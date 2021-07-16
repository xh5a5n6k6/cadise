#pragma once

#include "math/constant.h"
#include "math/type/mathType.h"

namespace cadise {

template<typename T>
class TVector2 {
public:
    static TVector2<T> min(const TVector2<T>& vectorA, const TVector2<T>& vectorB);
    static TVector2<T> max(const TVector2<T>& vectorA, const TVector2<T>& vectorB);

public:
    TVector2();
    explicit TVector2(const T value);
    TVector2(const T x, const T y);

    TVector2(const TVector2& other);
    template<typename U>
    explicit TVector2(const TVector2<U>& other);

    TVector2 operator + (const T rhs) const;
    TVector2 operator - (const T rhs) const;
    TVector2 operator * (const T rhs) const;
    TVector2 operator / (const T rhs) const;
    TVector2 operator + (const TVector2& rhs) const;
    TVector2 operator - (const TVector2& rhs) const;
    TVector2 operator * (const TVector2& rhs) const;
    TVector2 operator / (const TVector2& rhs) const;

    T& operator [] (const constant::AxisType index);
    const T& operator [] (const constant::AxisType index) const;

    template<typename U>
    TVector2<U> asType() const;

    TVector2  add(const TVector2& rhs) const;
    TVector2  add(const T rhs) const;
    TVector2  add(const T x, const T y) const;
    TVector2& addLocal(const TVector2& rhs);
    TVector2& addLocal(const T rhs);

    TVector2  sub(const TVector2& rhs) const;
    TVector2  sub(const T rhs) const;
    TVector2  sub(const T x, const T y) const;
    TVector2& subLocal(const TVector2& rhs);
    TVector2& subLocal(const T rhs);

    TVector2  mul(const TVector2& rhs) const;
    TVector2  mul(const T rhs) const;
    TVector2  mul(const T x, const T y) const;
    TVector2& mulLocal(const TVector2& rhs);
    TVector2& mulLocal(const T rhs);

    TVector2  div(const TVector2& rhs) const;
    TVector2  div(const T rhs) const;
    TVector2  div(const T x, const T y) const;
    TVector2& divLocal(const TVector2& rhs);
    TVector2& divLocal(const T rhs);

    bool isZero() const;
    bool hasNaN() const;
    bool hasNegative() const;
    bool hasInfinite() const;
    T sum() const;
    T average() const;
    T length() const;
    T lengthSquared() const;
    T product() const;
    T maxComponent() const;
    constant::AxisType maxDimension() const;

    void swap(TVector2& rhs);
    bool isEqualTo(const TVector2& rhs) const;

    T x() const;
    T y() const;

    void set(const TVector2& other);
    void set(const T value);
    void set(const T x, const T y);

private:
    T _x;
    T _y;
};

// template header implementation

template<typename T>
inline TVector2<T> operator + (const T scalar, const TVector2<T>& lhs) {
    return lhs.add(scalar);
}

template<typename T>
inline TVector2<T> operator * (const T scalar, const TVector2<T>& lhs) {
    return lhs.mul(scalar);
}

} // namespace cadise

#include "math/tVector2.ipp"