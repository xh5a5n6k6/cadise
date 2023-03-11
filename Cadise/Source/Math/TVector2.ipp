#pragma once

#include "Math/TVector2.h"

#include "Foundation/Assertion.h"

#include <cmath>
#include <type_traits>
#include <utility>

namespace cadise 
{

template<typename T>
inline TVector2<T> TVector2<T>::min(const TVector2<T>& vectorA, const TVector2<T>& vectorB)
{
    return TVector2<T>(
        std::min(vectorA._x, vectorB._x),
        std::min(vectorA._y, vectorB._y));
}

template<typename T>
inline TVector2<T> TVector2<T>::max(const TVector2<T>& vectorA, const TVector2<T>& vectorB) 
{
    return TVector2<T>(
        std::max(vectorA._x, vectorB._x),
        std::max(vectorA._y, vectorB._y));
}

template<typename T>
inline TVector2<T>::TVector2() :
    TVector2(static_cast<T>(0))
{}

template<typename T>
inline TVector2<T>::TVector2(const T value) :
    TVector2(value, value)
{}

template<typename T>
inline TVector2<T>::TVector2(const T x, const T y) :
    _x(x),
    _y(y) 
{}

template<typename T>
inline TVector2<T>::TVector2(const TVector2& other) = default;

template<typename T>
template<typename U>
inline TVector2<T>::TVector2(const TVector2<U>& other) :
    TVector2(
        static_cast<T>(other.x()),
        static_cast<T>(other.y())) 
{}

template<typename T>
inline TVector2<T> TVector2<T>::operator + (const T rhs) const
{
    return this->add(rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator - (const T rhs) const 
{
    return this->sub(rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator * (const T rhs) const 
{
    return this->mul(rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator / (const T rhs) const 
{
    return this->div(rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator + (const TVector2<T>& rhs) const
{
    return this->add(rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator - (const TVector2<T>& rhs) const 
{
    return this->sub(rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator * (const TVector2<T>& rhs) const 
{
    return this->mul(rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator / (const TVector2<T>& rhs) const 
{
    return this->div(rhs);
}

template<typename T>
inline T& TVector2<T>::operator [] (const constant::AxisType index) 
{
    CS_ASSERT_LT(index, 2);

    switch (index) 
    {
        case constant::AXIS_X:
            return _x;

        case constant::AXIS_Y:
            return _y;
    }

    CS_ASSERT(false);

    return _x;
}

template<typename T>
inline const T& TVector2<T>::operator [] (const constant::AxisType index) const 
{
    CS_ASSERT_LT(index, 2);

    switch (index) 
    {
        case constant::AXIS_X:
            return _x;

        case constant::AXIS_Y:
            return _y;
    }

    CS_ASSERT(false);

    return _x;
}

template<typename T>
template<typename U>
inline TVector2<U> TVector2<T>::asType() const 
{
    return TVector2<U>(*this);
}

template<typename T>
inline TVector2<T> TVector2<T>::add(const TVector2<T>& rhs) const 
{
    return this->add(rhs._x, rhs._y);
}

template<typename T>
inline TVector2<T> TVector2<T>::add(const T rhs) const 
{
    return this->add(rhs, rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::add(const T x, const T y) const 
{
    return TVector2<T>(_x + x, _y + y);
}

template<typename T>
inline TVector2<T>& TVector2<T>::addLocal(const TVector2<T>& rhs)
{
    _x += rhs._x;
    _y += rhs._y;

    return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::addLocal(const T rhs)
{
    _x += rhs;
    _y += rhs;

    return *this;
}

template<typename T>
inline TVector2<T> TVector2<T>::sub(const TVector2<T>& rhs) const 
{
    return this->sub(rhs._x, rhs._y);
}

template<typename T>
inline TVector2<T> TVector2<T>::sub(const T rhs) const
{
    return this->sub(rhs, rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::sub(const T x, const T y) const
{
    return TVector2<T>(_x - x, _y - y);
}

template<typename T>
inline TVector2<T>& TVector2<T>::subLocal(const TVector2<T>& rhs) 
{
    _x -= rhs._x;
    _y -= rhs._y;

    return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::subLocal(const T rhs) 
{
    _x -= rhs;
    _y -= rhs;

    return *this;
}

template<typename T>
inline TVector2<T> TVector2<T>::mul(const TVector2<T>& rhs) const 
{
    return this->mul(rhs._x, rhs._y);
}

template<typename T>
inline TVector2<T> TVector2<T>::mul(const T rhs) const
{
    return this->mul(rhs, rhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::mul(const T x, const T y) const 
{
    return TVector2<T>(_x * x, _y * y);
}

template<typename T>
inline TVector2<T>& TVector2<T>::mulLocal(const TVector2<T>& rhs)
{
    _x *= rhs._x;
    _y *= rhs._y;

    return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::mulLocal(const T rhs) 
{
    _x *= rhs;
    _y *= rhs;

    return *this;
}

template<typename T>
inline TVector2<T> TVector2<T>::div(const TVector2<T>& rhs) const 
{
    return this->div(rhs._x, rhs._y);
}

template<typename T>
inline TVector2<T> TVector2<T>::div(const T rhs) const 
{
    const T rcpRhs = static_cast<T>(1) / rhs;

    return this->mul(rcpRhs, rcpRhs);
}

template<typename T>
inline TVector2<T> TVector2<T>::div(const T x, const T y) const 
{
    return TVector2<T>(_x / x, _y / y);
}

template<typename T>
inline TVector2<T>& TVector2<T>::divLocal(const TVector2<T>& rhs)
{
    _x /= rhs._x;
    _y /= rhs._y;

    return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::divLocal(const T rhs) 
{
    const T rcpRhs = static_cast<T>(1) / rhs;

    _x *= rcpRhs;
    _y *= rcpRhs;

    return *this;
}

template<typename T>
inline bool TVector2<T>::isZero() const
{
    return _x == static_cast<T>(0) &&
           _y == static_cast<T>(0);
}

template<typename T>
inline bool TVector2<T>::hasNaN() const 
{
    return std::isnan(_x) || std::isnan(_y);
}

template<typename T>
inline bool TVector2<T>::hasNegative() const 
{
    static_assert(std::is_signed_v<T>,
        "TVector2<T>'s hasNegative function only works with signed type\n");

    return _x < static_cast<T>(0) ||
           _y < static_cast<T>(0);
}

template<typename T>
inline bool TVector2<T>::hasInfinite() const
{
    return !std::isfinite(_x) || !std::isfinite(_y);
}

template<typename T>
inline T TVector2<T>::sum() const 
{
    return _x + _y;
}

template<typename T>
inline T TVector2<T>::average() const 
{
    return this->sum() / static_cast<T>(2);
}

template<typename T>
inline T TVector2<T>::length() const
{
    return static_cast<T>(std::sqrt(this->lengthSquared()));
}

template<typename T>
inline T TVector2<T>::lengthSquared() const 
{
    return _x * _x + _y * _y;
}

template<typename T>
inline T TVector2<T>::product() const 
{
    return _x * _y;
}

template<typename T>
inline T TVector2<T>::maxComponent() const 
{
    return std::max(_x, _y);
}

template<typename T>
inline constant::AxisType TVector2<T>::maxDimension() const 
{
    return (_x > _y) ? constant::AXIS_X : constant::AXIS_Y;
}

template<typename T>
inline void TVector2<T>::swap(TVector2<T>& rhs) 
{
    std::swap(_x, rhs._x);
    std::swap(_y, rhs._y);
}

template<typename T>
inline bool TVector2<T>::isEqualTo(const TVector2<T>& rhs) const
{
    return _x == rhs._x && _y == rhs._y;
}

template<typename T>
inline T TVector2<T>::x() const 
{
    return _x;
}

template<typename T>
inline T TVector2<T>::y() const 
{
    return _y;
}

template<typename T>
inline void TVector2<T>::set(const TVector2<T>& other)
{
    this->set(other._x, other._y);
}

template<typename T>
inline void TVector2<T>::set(const T value)
{
    this->set(value, value);
}

template<typename T>
inline void TVector2<T>::set(const T x, const T y) 
{
    _x = x;
    _y = y;
}

} // namespace cadise