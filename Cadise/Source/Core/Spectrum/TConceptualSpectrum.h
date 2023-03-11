#pragma once

#include "Math/TArithmeticArray.h"

namespace cadise 
{

// TODO: refactor as CRTP (curiously recurring template pattern)
//       because which spectrum is used can be determined in compile time.
template<typename T, std::size_t N>
class TConceptualSpectrum
{
public:
    TConceptualSpectrum();
    explicit TConceptualSpectrum(const T value);
    explicit TConceptualSpectrum(const TArithmeticArray<T, N>& value);
    TConceptualSpectrum(const TConceptualSpectrum& other);

    virtual ~TConceptualSpectrum();

    TConceptualSpectrum operator + (const T rhs) const;
    TConceptualSpectrum operator - (const T rhs) const;
    TConceptualSpectrum operator * (const T rhs) const;
    TConceptualSpectrum operator / (const T rhs) const;
    TConceptualSpectrum operator + (const TConceptualSpectrum& rhs) const;
    TConceptualSpectrum operator - (const TConceptualSpectrum& rhs) const;
    TConceptualSpectrum operator * (const TConceptualSpectrum& rhs) const;
    TConceptualSpectrum operator / (const TConceptualSpectrum& rhs) const;
    
    TConceptualSpectrum  add(const TConceptualSpectrum& rhs) const;
    TConceptualSpectrum  add(const T rhs) const;
    TConceptualSpectrum& addLocal(const TConceptualSpectrum& rhs);
    TConceptualSpectrum& addLocal(const T rhs);

    TConceptualSpectrum  sub(const TConceptualSpectrum& rhs) const;
    TConceptualSpectrum  sub(const T rhs) const;
    TConceptualSpectrum& subLocal(const TConceptualSpectrum& rhs);
    TConceptualSpectrum& subLocal(const T rhs);

    TConceptualSpectrum  mul (const TConceptualSpectrum& rhs) const;
    TConceptualSpectrum  mul(const T rhs) const;
    TConceptualSpectrum& mulLocal(const TConceptualSpectrum& rhs);
    TConceptualSpectrum& mulLocal(const T rhs);

    TConceptualSpectrum  div(const TConceptualSpectrum& rhs) const;
    TConceptualSpectrum  div(const T rhs) const;
    TConceptualSpectrum& divLocal(const TConceptualSpectrum& rhs);
    TConceptualSpectrum& divLocal(const T rhs);

    bool isZero() const;
    bool hasNegative() const;
    bool hasNaN() const;
    bool hasInfinite() const;
    T sum() const;
    T average() const;
    T maxComponent() const;

    TConceptualSpectrum  squared() const;
    TConceptualSpectrum& squaredLocal();
    TConceptualSpectrum  complement() const;
    TConceptualSpectrum& complementLocal();

    void set(const TConceptualSpectrum& other);
    void set(const TArithmeticArray<T, N>& value);
    void set(const T value);
    
protected:
    TArithmeticArray<T, N> _values;
};

// template header implementation

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> operator + (const T scalar, const TConceptualSpectrum<T, N>& lhs)
{
    return lhs.add(scalar);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> operator * (const T scalar, const TConceptualSpectrum<T, N>& lhs)
{
    return lhs.mul(scalar);
}

} // namespace cadise

#include "Core/Spectrum/TConceptualSpectrum.ipp"