#pragma once

#include "Math/TArithmeticArray.h"

#include "Foundation/Assertion.h"

#include <cmath>
#include <utility>

namespace cadise
{

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>::TArithmeticArray() :
    TArithmeticArray(static_cast<T>(0))
{}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>::TArithmeticArray(const T value)
{
    _values.fill(value);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>::TArithmeticArray(const std::array<T, N>& value) :
    _values(value)
{}

template<typename T, std::size_t N>
template<typename... Ts>
inline TArithmeticArray<T, N>::TArithmeticArray(const T v1, const T v2, const Ts... ts) :
    _values({ v1, v2, ts... })
{}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>::TArithmeticArray(const TArithmeticArray<T, N>& other) = default;

template<typename T, std::size_t N>
template<typename U>
inline TArithmeticArray<T, N>::TArithmeticArray(const TArithmeticArray<U, N>& other)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] = static_cast<T>(other._values[i]);
    }
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::operator + (const T rhs) const
{
    return this->add(rhs);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::operator - (const T rhs) const
{
    return this->sub(rhs);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::operator * (const T rhs) const
{
    return this->mul(rhs);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::operator / (const T rhs) const
{
    return this->div(rhs);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::operator + (const TArithmeticArray<T, N>& rhs) const
{
    return this->add(rhs);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::operator - (const TArithmeticArray<T, N>& rhs) const
{
    return this->sub(rhs);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::operator * (const TArithmeticArray<T, N>& rhs) const
{
    return this->mul(rhs);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::operator / (const TArithmeticArray<T, N>& rhs) const
{
    return this->div(rhs);
}

template<typename T, std::size_t N>
inline T& TArithmeticArray<T, N>::operator [] (const std::size_t i)
{
    CS_ASSERT_LT(i, N);

    return _values[i];
}

template<typename T, std::size_t N>
inline const T& TArithmeticArray<T, N>::operator [] (const std::size_t i) const
{
    CS_ASSERT_LT(i, N);

    return _values[i];
}

template<typename T, std::size_t N>
template<typename U>
inline TArithmeticArray<U, N> TArithmeticArray<T, N>::asType() const
{
    return TArithmeticArray<U, N>(*this);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::add(const TArithmeticArray& rhs) const
{
    TArithmeticArray<T, N> result(*this);
    for (std::size_t i = 0; i < N; ++i)
    {
        result._values[i] += rhs._values[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::add(const T rhs) const
{
    TArithmeticArray<T, N> result(*this);
    for (std::size_t i = 0; i < N; ++i)
    {
        result._values[i] += rhs;
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::addLocal(const TArithmeticArray<T, N>& rhs)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] += rhs._values[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::addLocal(const T rhs)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] += rhs;
    }

    return *this;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::sub(const TArithmeticArray& rhs) const
{
    TArithmeticArray<T, N> result(*this);
    for (std::size_t i = 0; i < N; ++i)
    {
        result._values[i] -= rhs._values[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::sub(const T rhs) const
{
    TArithmeticArray<T, N> result(*this);
    for (std::size_t i = 0; i < N; ++i)
    {
        result._values[i] -= rhs;
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::subLocal(const TArithmeticArray<T, N>& rhs)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] -= rhs._values[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::subLocal(const T rhs)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] -= rhs;
    }

    return *this;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::mul(const TArithmeticArray& rhs) const
{
    TArithmeticArray<T, N> result(*this);
    for (std::size_t i = 0; i < N; ++i)
    {
        result._values[i] *= rhs._values[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::mul(const T rhs) const
{
    TArithmeticArray<T, N> result(*this);
    for (std::size_t i = 0; i < N; ++i)
    {
        result._values[i] *= rhs;
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::mulLocal(const TArithmeticArray<T, N>& rhs)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] *= rhs._values[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::mulLocal(const T rhs)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] *= rhs;
    }

    return *this;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::div(const TArithmeticArray& rhs) const
{
    TArithmeticArray<T, N> result(*this);
    for (std::size_t i = 0; i < N; ++i)
    {
        result._values[i] /= rhs._values[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::div(const T rhs) const
{
    const T rcpRhs = static_cast<T>(1) / rhs;

    TArithmeticArray<T, N> result(*this);
    for (std::size_t i = 0; i < N; ++i)
    {
        result._values[i] *= rcpRhs;
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::divLocal(const TArithmeticArray<T, N>& rhs)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] /= rhs._values[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::divLocal(const T rhs)
{
    const T rcpRhs = static_cast<T>(1) / rhs;

    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] *= rcpRhs;
    }

    return *this;
}

template<typename T, std::size_t N>
inline bool TArithmeticArray<T, N>::isZero() const
{
    for (std::size_t i = 0; i < N; ++i)
    {
        if (_values[i] != static_cast<T>(0))
        {
            return false;
        }
    }

    return true;
}

template<typename T, std::size_t N>
inline bool TArithmeticArray<T, N>::hasNaN() const
{
    for (std::size_t i = 0; i < N; ++i)
    {
        if (std::isnan(_values[i]))
        {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t N>
inline bool TArithmeticArray<T, N>::hasNegative() const
{
    for (std::size_t i = 0; i < N; ++i)
    {
        if (_values[i] < static_cast<T>(0))
        {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t N>
inline bool TArithmeticArray<T, N>::hasInfinite() const
{
    for (std::size_t i = 0; i < N; ++i)
    {
        if (!std::isfinite(_values[i]))
        {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t N>
inline T TArithmeticArray<T, N>::sum() const
{
    T result = _values[0];
    for (std::size_t i = 1; i < N; ++i)
    {
        result += _values[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline T TArithmeticArray<T, N>::average() const
{
    return this->sum() / static_cast<T>(N);
}

template<typename T, std::size_t N>
inline T TArithmeticArray<T, N>::maxComponent() const
{
    T result = _values[0];
    for (std::size_t i = 1; i < N; ++i)
    {
        result = std::max(result, _values[i]);
    }

    return result;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::squared() const
{
    return TArithmeticArray<T, N>(*this).mul(*this);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::squaredLocal()
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] *= _values[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> TArithmeticArray<T, N>::complement() const
{
    return TArithmeticArray<T, N>(static_cast<T>(1)).sub(*this);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N>& TArithmeticArray<T, N>::complementLocal()
{
    for (std::size_t i = 0; i < N; ++i)
    {
        _values[i] = static_cast<T>(1) - _values[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline bool TArithmeticArray<T, N>::isEqualTo(const TArithmeticArray<T, N>& rhs) const
{
    for (std::size_t i = 0; i < N; ++i)
    {
        if (_values[i] != rhs._values[i])
        {
            return false;
        }
    }

    return true;
}

template<typename T, std::size_t N>
inline void TArithmeticArray<T, N>::set(const TArithmeticArray<T, N>& other)
{
    _values = other._values;
}

template<typename T, std::size_t N>
inline void TArithmeticArray<T, N>::set(const T value)
{
    _values.fill(value);
}

} // namespace cadise