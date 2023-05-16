#pragma once

#include <array>

namespace cadise
{

template<typename T, std::size_t N>
class TArithmeticArray
{
public:
    TArithmeticArray();
    explicit TArithmeticArray(const T value);
    explicit TArithmeticArray(const std::array<T, N>& value);
    template<typename... Ts>
    TArithmeticArray(const T vA, const T vB, const Ts... ts);

    TArithmeticArray(const TArithmeticArray& other);
    template<typename U>
    explicit TArithmeticArray(const TArithmeticArray<U, N>& other);

    TArithmeticArray operator + (const T rhs) const;
    TArithmeticArray operator - (const T rhs) const;
    TArithmeticArray operator * (const T rhs) const;
    TArithmeticArray operator / (const T rhs) const;
    TArithmeticArray operator + (const TArithmeticArray& rhs) const;
    TArithmeticArray operator - (const TArithmeticArray& rhs) const;
    TArithmeticArray operator * (const TArithmeticArray& rhs) const;
    TArithmeticArray operator / (const TArithmeticArray& rhs) const;

    T& operator [] (const std::size_t index);
    const T& operator [] (const std::size_t index) const;

    template<typename U>
    TArithmeticArray<U, N> asType() const;

    TArithmeticArray  add(const TArithmeticArray& rhs) const;
    TArithmeticArray  add(const T rhs) const;
    TArithmeticArray& addLocal(const TArithmeticArray& rhs);
    TArithmeticArray& addLocal(const T rhs);

    TArithmeticArray  sub(const TArithmeticArray& rhs) const;
    TArithmeticArray  sub(const T rhs) const;
    TArithmeticArray& subLocal(const TArithmeticArray& rhs);
    TArithmeticArray& subLocal(const T rhs);

    TArithmeticArray  mul(const TArithmeticArray& rhs) const;
    TArithmeticArray  mul(const T rhs) const;
    TArithmeticArray& mulLocal(const TArithmeticArray& rhs);
    TArithmeticArray& mulLocal(const T rhs);

    TArithmeticArray  div(const TArithmeticArray& rhs) const;
    TArithmeticArray  div(const T rhs) const;
    TArithmeticArray& divLocal(const TArithmeticArray& rhs);
    TArithmeticArray& divLocal(const T rhs);

    bool isZero() const;
    bool hasNaN() const;
    bool hasNegative() const;
    bool hasInfinite() const;
    T sum() const;
    T average() const;
    T maxComponent() const;

    TArithmeticArray  squared() const;
    TArithmeticArray& squaredLocal();
    TArithmeticArray  complement() const;
    TArithmeticArray& complementLocal();

    bool isEqualTo(const TArithmeticArray& rhs) const;

    void set(const TArithmeticArray& other);
    void set(const T value);

private:
    std::array<T, N> _values;
};

// template header implementation

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> operator + (const T scalar, const TArithmeticArray<T, N>& lhs)
{
    return lhs.add(scalar);
}

template<typename T, std::size_t N>
inline TArithmeticArray<T, N> operator * (const T scalar, const TArithmeticArray<T, N>& lhs)
{
    return lhs.mul(scalar);
}

} // namespace cadise

#include "Math/TArithmeticArray.ipp"