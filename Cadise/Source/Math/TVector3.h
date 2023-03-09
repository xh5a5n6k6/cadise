#pragma once

#include "math/constant.h"
#include "math/type/mathType.h"

namespace cadise 
{

template<typename T>
class TVector3 
{
public:
    static TVector3 min(const TVector3& vectorA, const TVector3& vectorB);
    static TVector3 max(const TVector3& vectorA, const TVector3& vectorB);

public:
    TVector3();
    explicit TVector3(const T value);
    explicit TVector3(const TArithmeticArray<T, 3>& value);
    TVector3(const T x, const T y, const T z);

    TVector3(const TVector3& other);
    template<typename U>
    explicit TVector3(const TVector3<U>& other);

    TVector3 operator + (const T rhs) const;
    TVector3 operator - (const T rhs) const;
    TVector3 operator * (const T rhs) const;
    TVector3 operator / (const T rhs) const;
    TVector3 operator + (const TVector3& rhs) const;
    TVector3 operator - (const TVector3& rhs) const;
    TVector3 operator * (const TVector3& rhs) const;
    TVector3 operator / (const TVector3& rhs) const;

    T& operator [] (const constant::AxisType index);
    const T& operator [] (const constant::AxisType index) const;
    
    template<typename U>
    TVector3<U> asType() const;

    TVector3  add(const TVector3& rhs) const;
    TVector3  add(const T rhs) const;
    TVector3  add(const T x, const T y, const T z) const;
    TVector3& addLocal(const TVector3& rhs);
    TVector3& addLocal(const T rhs);

    TVector3  sub(const TVector3& rhs) const;
    TVector3  sub(const T rhs) const;
    TVector3  sub(const T x, const T y, const T z) const;
    TVector3& subLocal(const TVector3& rhs);
    TVector3& subLocal(const T rhs);

    TVector3  mul(const TVector3& rhs) const;
    TVector3  mul(const T rhs) const;
    TVector3  mul(const T x, const T y, const T z) const;
    TVector3& mulLocal(const TVector3& rhs);
    TVector3& mulLocal(const T rhs);

    TVector3  div(const TVector3& rhs) const;
    TVector3  div(const T rhs) const;
    TVector3  div(const T x, const T y, const T z) const;
    TVector3& divLocal(const TVector3& rhs);
    TVector3& divLocal(const T rhs);

    bool isZero() const;
    bool hasNaN() const;
    bool hasNegative() const;
    bool hasInfinite() const;
    T sum() const;
    T average() const;
    T length() const;
    T lengthSquared() const;
    T maxComponent() const;
    constant::AxisType maxDimension() const;

    TVector3  normalize() const;
    TVector3& normalizeLocal();
    TVector3  negate() const;
    TVector3& negateLocal();
    TVector3  reciprocal() const;
    TVector3& reciprocalLocal();
    TVector3  clamp(const T min, const T max) const;
    TVector3& clampLocal(const T min, const T max);
    
    void swap(TVector3& rhs);
    bool isEqualTo(const TVector3& rhs) const;
    T dot(const TVector3& rhs) const;
    T absDot(const TVector3& rhs) const;
    TVector3 lerp(const TVector3& rhs, const T ratio) const;
    TVector3 cross(const TVector3& rhs) const;
    TVector3 reflect(const TVector3& normal) const;

    bool canRefract(
        const TVector3& normal,
        const real      iorOuter,
        const real      iorInner,
        TVector3* const out_vector) const;

    T x() const;
    T y() const;
    T z() const;

    void set(const TVector3& other);
    void set(const T value);
    void set(const T x, const T y, const T z);

private:
    T _x;
    T _y;
    T _z;
};

// template header implementation

template<typename T>
inline TVector3<T> operator + (const T scalar, const TVector3<T>& lhs) 
{
    return lhs.add(scalar);
}

template<typename T>
inline TVector3<T> operator * (const T scalar, const TVector3<T>& lhs) 
{
    return lhs.mul(scalar);
}

} // namespace cadise

#include "math/tVector3.ipp"