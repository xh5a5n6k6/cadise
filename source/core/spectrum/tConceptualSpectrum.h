#pragma once

#include "math/tVector.h"

namespace cadise {

// TODO: refactor as CRTP (curiously recurring template pattern)
template<std::size_t N>
class TConceptualSpectrum {
protected:
    using SpectralArray = TVector<real, N>;

public:
    TConceptualSpectrum();
    explicit TConceptualSpectrum(const real v);
    explicit TConceptualSpectrum(const SpectralArray& sa);
    TConceptualSpectrum(const TConceptualSpectrum& cs);

    virtual ~TConceptualSpectrum();

    TConceptualSpectrum operator - () const;
    TConceptualSpectrum operator + (const real s) const;
    TConceptualSpectrum operator - (const real s) const;
    TConceptualSpectrum operator * (const real s) const;
    TConceptualSpectrum operator / (const real s) const;
    TConceptualSpectrum operator + (const TConceptualSpectrum& cs) const;
    TConceptualSpectrum operator - (const TConceptualSpectrum& cs) const;
    TConceptualSpectrum operator * (const TConceptualSpectrum& cs) const;
    TConceptualSpectrum operator / (const TConceptualSpectrum& cs) const;
    TConceptualSpectrum& operator += (const real s);
    TConceptualSpectrum& operator -= (const real s);
    TConceptualSpectrum& operator *= (const real s);
    TConceptualSpectrum& operator /= (const real s);
    TConceptualSpectrum& operator += (const TConceptualSpectrum& cs);
    TConceptualSpectrum& operator -= (const TConceptualSpectrum& cs);
    TConceptualSpectrum& operator *= (const TConceptualSpectrum& cs);
    TConceptualSpectrum& operator /= (const TConceptualSpectrum& cs);
    TConceptualSpectrum& operator  = (const TConceptualSpectrum& cs);

    bool isZero() const;
    bool hasNegative() const;
    bool hasNaN() const;
    bool hasInfinite() const;
    real sum() const;
    real average() const;
    real maxComponent() const;

    TConceptualSpectrum square() const;
    TConceptualSpectrum complement() const;

protected:
    SpectralArray _values;
};

// template header implementation

template<std::size_t N>
inline TConceptualSpectrum<N> operator + (const real s, const TConceptualSpectrum<N>& cs) {
    return cs + s;
}

template<std::size_t N>
inline TConceptualSpectrum<N> operator * (const real s, const TConceptualSpectrum<N>& cs) {
    return cs * s;
}

} // namespace cadise

#include "core/spectrum/tConceptualSpectrum.ipp"